#include <dls2/external_process/managed_external_process.hpp>

#include <csignal>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std::chrono_literals;
namespace fs = std::filesystem;
using dls::ManagedExternalProcess;

namespace
{
    volatile std::sig_atomic_t interrupted = 0;
    void interrupt(int) { interrupted = 1; }
    void require(bool condition, const char* message)
    {
        if (!condition) throw std::runtime_error(message);
    }
    template<class Predicate> void await(Predicate predicate)
    {
        const auto deadline = std::chrono::steady_clock::now() + 3s;
        while (!predicate())
        {
            require(std::chrono::steady_clock::now() < deadline, "Timed out");
            std::this_thread::sleep_for(5ms);
        }
    }

    int fixture(const std::string& mode, const std::string& ready)
    {
        std::signal(SIGINT, (mode == "graceful" || mode == "policy") ? interrupt : SIG_IGN);
        std::signal(SIGTERM, SIG_IGN);
        const auto node = ::fork();
        if (node == 0)
        {
            if (mode == "policy") {
                while (!interrupted) std::this_thread::sleep_for(5ms);
                std::ofstream(ready + ".node-interrupted") << "SIGINT\n";
                return 0;
            }
            std::signal(SIGINT, SIG_IGN);
            for (;;) ::pause();
        }
        require(node > 0, "fork failed");
        { std::ofstream output(ready); output << ::getpid() << ' ' << node << '\n'; }
        if (mode == "orphan") return 0;
        while (!interrupted) std::this_thread::sleep_for(5ms);
        if (mode == "policy") std::this_thread::sleep_for(100ms);
        ::kill(node, SIGKILL);
        ::waitpid(node, nullptr, 0);
        return 0;
    }

    void checkGone(pid_t launcher, pid_t node)
    {
        // Adopt grandchildren so the test can reap them like a container init.
        await([&] {
            while (::waitpid(-1, nullptr, WNOHANG) > 0) {}
            return ::kill(launcher, 0) == -1 && ::kill(node, 0) == -1;
        });
    }
}

int main(int argc, char** argv)
{
    if (argc == 4) return fixture(argv[2], argv[3]);
    require(::prctl(PR_SET_CHILD_SUBREAPER, 1) == 0, "subreaper failed");
    const auto executable = fs::canonical("/proc/self/exe").string();
    const auto directory = fs::temp_directory_path() / ("dls-process-test-" + std::to_string(::getpid()));
    fs::create_directory(directory);
    ManagedExternalProcess process;
    int cycle = 0;
    for (const auto& mode : {"graceful", "stubborn", "orphan", "graceful"})
    {
        std::cout << "Testing " << mode << " shutdown, cycle " << cycle << std::endl;
        const auto ready = (directory / std::to_string(cycle++)).string();
        const std::vector<std::string> command{executable, "fixture", mode, ready};
        process.start(command, 100ms, 100ms);
        // Repeated activation must not spawn another process.
        process.start({"does-not-exist"}, 100ms, 100ms);
        pid_t launcher = 0, node = 0;
        await([&] { std::ifstream input(ready); return bool(input >> launcher >> node); });
        require(::getpgid(launcher) != ::getpgrp(), "Shared process group");
        require(::getsid(launcher) == launcher, "Child did not create a dedicated session");
        if (std::string(mode) == "orphan") await([&] { return !process.running(); });
        const auto before = std::chrono::steady_clock::now();
        process.requestStop();
        require(std::chrono::steady_clock::now() - before < 100ms, "Stop request blocked");
        await([&] { process.requestStop(); return process.stopComplete(); });
        require(!process.running(), "Launcher still running");
        process.stopAndWait();
        checkGone(launcher, node);
    }
    std::cout << "Testing destructor cleanup" << std::endl;
    pid_t launcher = 0, node = 0;
    {
        ManagedExternalProcess scoped;
        const auto ready = (directory / "destructor").string();
        scoped.start({executable, "fixture", "stubborn", ready}, 50ms, 50ms);
        await([&] { std::ifstream input(ready); return bool(input >> launcher >> node); });
    }
    checkGone(launcher, node);
    std::cout << "Testing missing executable" << std::endl;
    bool rejected = false;
    try { process.start({"dls-missing-executable-for-process-test"}, 50ms, 50ms); }
    catch (const std::exception&) { rejected = true; }
    require(rejected, "Missing executable was accepted");
    process.stopAndWait();

    for (const auto target : {dls::utils::InitialSignalTarget::leader,
                              dls::utils::InitialSignalTarget::group}) {
        const bool whole_group = target == dls::utils::InitialSignalTarget::group;
        std::cout << "Testing initial signal target: " << (whole_group ? "group" : "leader") << std::endl;
        const auto ready = (directory / (whole_group ? "group-policy" : "leader-policy")).string();
        auto owned = std::make_shared<dls::utils::OwnedProcess>(
            std::vector<std::string>{executable, "fixture", "policy", ready});
        await([&] { std::ifstream input(ready); return bool(input >> launcher >> node); });
        dls::utils::OwnedProcesses children{{"policy fixture", owned}};
        dls::utils::ProcessShutdownOptions options;
        options.initial_target = target;
        options.interrupt_timeout = 500ms;
        options.terminate_timeout = 100ms;
        options.kill_timeout = 500ms;
        require(dls::utils::shutdownProcesses(children, options), "Policy shutdown failed");
        require(fs::exists(ready + ".node-interrupted") == whole_group,
                "SIGINT reached the wrong process set");
        checkGone(launcher, node);
    }

    std::cout << "Testing group liveness after leader exit and legacy shutdown API" << std::endl;
    {
        const auto ready = (directory / "owned-orphan").string();
        auto owned = std::make_shared<dls::utils::OwnedProcess>(
            std::vector<std::string>{executable, "fixture", "orphan", ready});
        await([&] { std::ifstream input(ready); return bool(input >> launcher >> node); });
        await([&] { return !owned->leaderRunning(); });
        require(owned->running(), "Surviving descendant was not tracked");
        dls::utils::OwnedProcesses children{{"orphan fixture", owned}};
        require(dls::utils::shutdownProcesses(children, 10ms), "Legacy shutdown failed");
        checkGone(launcher, node);
    }
    fs::remove_all(directory);
}
