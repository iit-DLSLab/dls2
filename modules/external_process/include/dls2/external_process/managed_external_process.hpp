#ifndef MANAGED_EXTERNAL_PROCESS_HPP
#define MANAGED_EXTERNAL_PROCESS_HPP

#include <boost/process.hpp>
#include <chrono>
#include <future>
#include <memory>
#include <string>
#include <vector>

namespace dls
{
    // Lifecycle methods are called serially by the plugin's state-machine thread.
    class ManagedExternalProcess
    {
    public:
        ManagedExternalProcess() = default;
        ~ManagedExternalProcess();
        ManagedExternalProcess(const ManagedExternalProcess&) = delete;
        ManagedExternalProcess& operator=(const ManagedExternalProcess&) = delete;

        void start(const std::vector<std::string>& command,
                   std::chrono::milliseconds interrupt_timeout,
                   std::chrono::milliseconds terminate_timeout);
        bool running();
        void requestStop();
        bool stopComplete();
        void stopAndWait();

    private:
        void stop();
        bool waitForGroup(std::chrono::milliseconds timeout);
        void signalGroup(int signal);

        std::unique_ptr<boost::process::group> group_;
        std::unique_ptr<boost::process::child> child_;
        std::shared_future<void> stop_result_;
        bool shutdown_sent_{false};
        std::chrono::milliseconds interrupt_timeout_{15000};
        std::chrono::milliseconds terminate_timeout_{5000};
    };
}
#endif
