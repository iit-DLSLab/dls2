#ifndef PY_PERIODIC_APP_PLUGIN_HPP
#define PY_PERIODIC_APP_PLUGIN_HPP

#include <atomic>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>

#include <dls2/plugin/periodic_app_plugin.hpp>

namespace dls
{
enum class TimeoutPolicy
{
    HoldLast,
    Zero,
    Fail,
    Deactivate
};

struct BridgeRuntimeConfig
{
    std::chrono::milliseconds command_timeout_ms{300};
    TimeoutPolicy timeout_policy{TimeoutPolicy::Fail};
};

TimeoutPolicy timeoutPolicyFromString(const std::string& policy);

class PyPeriodicAppPlugin : public dls::PeriodicAppPlugin
{
public:
    PyPeriodicAppPlugin(const std::string& id, BridgeRuntimeConfig runtime_config);
    ~PyPeriodicAppPlugin() override;

    void start();
    void activate();
    void deactivate();
    void shutdown();

    bool isStarted() const;
    std::string stateName();

    bool checkActivation() override;
    void run(const std::chrono::system_clock::time_point& time) final;

protected:
    void markPythonCommandReceived();

    virtual void captureInputSnapshot(const std::chrono::system_clock::time_point& time) = 0;
    virtual void applyLatestCommand() = 0;
    virtual void applyTimeoutCommand(TimeoutPolicy policy) = 0;

private:
    bool hasFreshPythonCommand(std::chrono::steady_clock::time_point now) const;

    BridgeRuntimeConfig runtime_config_;
    mutable std::mutex command_stamp_mutex_;
    std::chrono::steady_clock::time_point last_python_command_stamp_{};
    bool has_python_command_{false};

    std::thread app_thread_;
    std::atomic<bool> started_{false};
    std::atomic<bool> shutdown_requested_{false};
};
} // namespace dls

#endif // PY_PERIODIC_APP_PLUGIN_HPP
