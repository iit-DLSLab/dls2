#include "dls2/external_process/managed_external_process.hpp"

#include <cerrno>
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <thread>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

using namespace dls;

ManagedExternalProcess::~ManagedExternalProcess()
{
    try { stopAndWait(); }
    catch (const std::exception& error)
    {
        std::cerr << "ManagedExternalProcess shutdown failed: " << error.what() << '\n';
        // Attached Boost handles provide a final forced-termination fallback.
    }
}

void ManagedExternalProcess::start(const std::vector<std::string>& command,
                            std::chrono::milliseconds interrupt_timeout,
                            std::chrono::milliseconds terminate_timeout)
{
    if (child_ && !shutdown_sent_)
        return;

    if (shutdown_sent_) 
        stop_result_.get();

    if (command.empty() || command.front().empty())
        throw std::invalid_argument("Configure launch command string");
    
    if (interrupt_timeout.count() < 0 || terminate_timeout.count() < 0)
        throw std::invalid_argument("ManagedExternalProcess shutdown timeouts must be nonnegative");

    const auto executable = command.front().find('/') == std::string::npos
        ? boost::process::search_path(command.front())
        : boost::filesystem::path(command.front());

    if (executable.empty())
        throw std::runtime_error("Launch executable not found: " + command.front());

    auto group = std::make_unique<boost::process::group>();
    auto child = std::make_unique<boost::process::child>(
        executable, boost::process::args(std::vector<std::string>(command.begin() + 1, command.end())),
        *group);

    group_ = std::move(group);
    child_ = std::move(child);
    interrupt_timeout_ = interrupt_timeout;
    terminate_timeout_ = terminate_timeout;
    stop_result_ = {};
    shutdown_sent_ = false;
}

bool ManagedExternalProcess::running()
{
    return !shutdown_sent_ && child_ && child_->running();
}

void ManagedExternalProcess::requestStop()
{
    if (shutdown_sent_) return;
    stop_result_ = std::async(std::launch::async, [this] { stop(); }).share();
    shutdown_sent_ = true;
}

bool ManagedExternalProcess::stopComplete()
{
    if (!shutdown_sent_) return false;
    if (stop_result_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
        return false;

    stop_result_.get();
    return true;
}

void ManagedExternalProcess::stopAndWait()
{
    requestStop();
    stop_result_.get();
}

void ManagedExternalProcess::signalGroup(int signal)
{
    const auto pgid = group_->native_handle();
    if (pgid <= 0 || pgid == ::getpgrp())
        throw std::runtime_error("Refusing to signal an invalid or shared process group");
    if (::kill(-pgid, signal) != 0 && errno != ESRCH)
        throw std::system_error(errno, std::generic_category(), "Signal external process group");
}

bool ManagedExternalProcess::waitForGroup(std::chrono::milliseconds timeout)
{
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    do
    {
        // Reap the launcher when it exits, but still check for surviving nodes.
        child_->running();
        if (::kill(-group_->native_handle(), 0) != 0)
        {
            if (errno == ESRCH) return true;
            throw std::system_error(errno, std::generic_category(), "Check external process group");
        }
        if (std::chrono::steady_clock::now() >= deadline) return false;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    } while (true);
}

void ManagedExternalProcess::stop()
{
    if (!child_) return;
    // A worker created during deactivation may inherit the periodic policy.
    sched_param parameters{};
    const int scheduler_error = ::pthread_setschedparam(::pthread_self(), SCHED_OTHER, &parameters);
    if (scheduler_error != 0)
        std::cerr << "External group shutdown worker could not select SCHED_OTHER: "
                    << std::generic_category().message(scheduler_error) << '\n';
    // Give ROS launch the first opportunity to shut down its own nodes.
    if (child_->running() && ::kill(child_->id(), SIGINT) != 0 && errno != ESRCH)
        throw std::system_error(errno, std::generic_category(), "Interrupt ROS launch");
    if (!waitForGroup(interrupt_timeout_))
    {
        signalGroup(SIGTERM);
        if (!waitForGroup(terminate_timeout_)) signalGroup(SIGKILL);
    }
    child_->wait();
    // Signals have been delivered to all remaining group members. Grandchild
    // zombies must be reaped by their parent/init, not by this plugin.
    group_->detach();
    child_.reset();
    group_.reset();
}
