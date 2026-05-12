#include "dls2/plugin/py_periodic_app_plugin.hpp"

#include <stdexcept>

namespace dls
{
TimeoutPolicy timeoutPolicyFromString(const std::string& policy)
{
    if (policy == "hold_last")
    {
        return TimeoutPolicy::HoldLast;
    }
    if (policy == "zero")
    {
        return TimeoutPolicy::Zero;
    }
    if (policy == "fail")
    {
        return TimeoutPolicy::Fail;
    }
    if (policy == "deactivate")
    {
        return TimeoutPolicy::Deactivate;
    }
    throw std::runtime_error("Unknown timeout policy: " + policy);
}

PyPeriodicAppPlugin::PyPeriodicAppPlugin(const std::string& id,
                                       BridgeRuntimeConfig runtime_config)
    : dls::PeriodicAppPlugin(id)
    , runtime_config_(runtime_config)
{}

PyPeriodicAppPlugin::~PyPeriodicAppPlugin()
{
    shutdown();
}

void PyPeriodicAppPlugin::start()
{
    bool expected = false;
    if (!started_.compare_exchange_strong(expected, true))
    {
        return;
    }

    app_thread_ = std::thread([this]() {
        this->execute();
    });
}

void PyPeriodicAppPlugin::activate()
{
    sm.raiseEvent(sm.activation_request);
}

void PyPeriodicAppPlugin::deactivate()
{
    sm.raiseEvent(sm.deactivation_request);
}

void PyPeriodicAppPlugin::shutdown()
{
    if (!started_.load() || shutdown_requested_.exchange(true))
    {
        return;
    }

    stop();

    if (app_thread_.joinable())
    {
        app_thread_.join();
    }
}

bool PyPeriodicAppPlugin::isStarted() const
{
    return started_.load();
}

std::string PyPeriodicAppPlugin::stateName()
{
    return sm.getStateName();
}

bool PyPeriodicAppPlugin::checkActivation()
{
    return basicActivationChecks();
}

void PyPeriodicAppPlugin::run(const std::chrono::system_clock::time_point& time)
{
    read();
    captureInputSnapshot(time);

    const auto now = std::chrono::steady_clock::now();
    std::cout << "Current state: " << stateName() << ", has fresh python command: " << hasFreshPythonCommand(now) << std::endl;
    if (hasFreshPythonCommand(now))
    {
        applyLatestCommand();
    }
    else
    {
        applyTimeoutCommand(runtime_config_.timeout_policy);
    }

    write();
}

void PyPeriodicAppPlugin::markPythonCommandReceived()
{
    std::lock_guard<std::mutex> lock(command_stamp_mutex_);
    has_python_command_ = true;
    last_python_command_stamp_ = std::chrono::steady_clock::now();
}

bool PyPeriodicAppPlugin::hasFreshPythonCommand(std::chrono::steady_clock::time_point now) const
{
    std::lock_guard<std::mutex> lock(command_stamp_mutex_);
    return has_python_command_ && (now - last_python_command_stamp_) <= runtime_config_.command_timeout_ms;
}
} // namespace dls
