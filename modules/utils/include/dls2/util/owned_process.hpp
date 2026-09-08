#pragma once

#include <boost/process.hpp>
#include <boost/process/extend.hpp>
#include <chrono>
#include <cerrno>
#include <csignal>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace dls::utils
{
// Descendants inherit this dedicated group, including exec'd Python plugins.
// The owning executable must enable PR_SET_CHILD_SUBREAPER to reap orphaned
// descendants after a layer exits before its plugins.
class OwnedProcess
{
public:
    // The interactive console must retain the terminal's foreground group.
    explicit OwnedProcess(const std::vector<std::string>& args, bool own_group = true)
        : proc(own_group ? boost::process::child(args,
              boost::process::extend::on_exec_setup = [](auto& executor) {
                  // Keep inherited stdin/stdout, but detach from terminal job control.
                  // setpgid alone lets keyboard I/O suspend the entire layer group.
                  if (::setsid() == -1)
                      executor.set_error(std::error_code(errno, std::generic_category()), "setsid");
              }) : boost::process::child(args)),
          own_group_(own_group)
    {
        if (own_group_) {
            auto pgid = proc.id(); // setsid creates a group with the child's PID.
            group_ = boost::process::group(pgid);
        }
    }

    bool running()
    {
        if (finished_) return false;
        if (!leader_exited_) {
            std::error_code error;
            leader_exited_ = !proc.running(error);
            if (error) throw std::system_error(error, "checking owned child");
        }
        if (!own_group_) return !leader_exited_;
        if (leader_exited_) {
            int status;
            while (::waitpid(-group_.native_handle(), &status, WNOHANG) > 0) {}
        }
        if (::kill(-group_.native_handle(), 0) == 0 || errno != ESRCH) return true;
        finished_ = true;
        group_.detach();
        return false;
    }

    void signal(int value)
    {
        if (!finished_ && ::kill(own_group_ ? -group_.native_handle() : proc.id(), value) != 0 && errno != ESRCH)
            throw std::system_error(errno, std::generic_category(), "signalling owned process group");
    }

    int id() const { return proc.id(); }
    void detach() { proc.detach(); group_.detach(); }

private:
    boost::process::group group_;
public:
    boost::process::child proc;
private:
    bool leader_exited_{false};
    bool finished_{false};
    bool own_group_;
};

using OwnedProcesses = std::map<std::string, std::shared_ptr<OwnedProcess>>;

inline bool shutdownProcesses(OwnedProcesses& processes, std::chrono::milliseconds grace)
{
    using namespace std::chrono_literals;
    const auto wait = [&](std::chrono::milliseconds duration) {
        const auto deadline = std::chrono::steady_clock::now() + duration;
        for (;;) {
            bool any_running = false;
            for (auto& [name, process] : processes) any_running |= process->running();
            if (!any_running) return true;
            if (std::chrono::steady_clock::now() >= deadline) return false;
            std::this_thread::sleep_for(20ms);
        }
    };
    const auto signal = [&](int value, bool report) {
        for (auto& [name, process] : processes) {
            if (!process->running()) continue;
            if (report) std::cerr << "Shutdown: " << name << " (PID/PGID " << process->id()
                                  << ") still running; sending signal " << value << std::endl;
            process->signal(value);
        }
    };
    signal(SIGINT, false);
    if (wait(grace)) return true;
    signal(SIGTERM, true);
    if (wait(2s)) return true;
    signal(SIGKILL, true);
    if (wait(2s)) return true;
    for (auto& [name, process] : processes) {
        if (process->running()) {
            std::cerr << "Shutdown: " << name << " (PID/PGID " << process->id()
                      << ") did not exit after SIGKILL" << std::endl;
            process->detach(); // Do not turn a bounded shutdown into a destructor wait.
        }
    }
    return false;
}
}
