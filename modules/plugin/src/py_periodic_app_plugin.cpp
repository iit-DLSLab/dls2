#include "dls2/plugin/py_periodic_app_plugin.hpp"

namespace dls
{
PyPeriodicAppPlugin::PyPeriodicAppPlugin(const std::string& id)
    : dls::PeriodicAppPlugin(id)
{
}

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

void PyPeriodicAppPlugin::serve()
{
    start();
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
    return basicActivationChecks() && pythonCheckActivation();
}

bool PyPeriodicAppPlugin::pythonCheckActivation()
{
    return true;
}

void PyPeriodicAppPlugin::run(const std::chrono::system_clock::time_point&)
{
    read();

    py::gil_scoped_acquire acquire;
    py::object py_input = makePythonInput();
    py::object py_output = computeCommand(py_input);
    applyPythonOutput(py_output);

    write();
}
} // namespace dls
