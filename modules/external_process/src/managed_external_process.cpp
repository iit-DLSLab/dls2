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
    if (process_ && !shutdown_sent_)
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

    auto args = command;
    args.front() = executable.string();
    process_ = std::make_shared<utils::OwnedProcess>(args);
    interrupt_timeout_ = interrupt_timeout;
    terminate_timeout_ = terminate_timeout;
    stop_result_ = {};
    shutdown_sent_ = false;
}

bool ManagedExternalProcess::running()
{
    return !shutdown_sent_ && process_ && process_->leaderRunning();
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

void ManagedExternalProcess::stop()
{
    if (!process_) return;
    // A worker created during deactivation may inherit the periodic policy.
    sched_param parameters{};
    const int scheduler_error = ::pthread_setschedparam(::pthread_self(), SCHED_OTHER, &parameters);
    if (scheduler_error != 0)
        std::cerr << "External group shutdown worker could not select SCHED_OTHER: "
                    << std::generic_category().message(scheduler_error) << '\n';
    utils::ProcessShutdownOptions options;
    options.interrupt_timeout = interrupt_timeout_;
    options.terminate_timeout = terminate_timeout_;
    options.initial_target = utils::InitialSignalTarget::leader;
    utils::OwnedProcesses processes{{"external process", process_}};
    if (!utils::shutdownProcesses(processes, options))
        throw std::runtime_error("External process group did not exit after SIGKILL");
    process_.reset();
}
