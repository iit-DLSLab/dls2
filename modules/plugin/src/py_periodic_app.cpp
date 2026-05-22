#include "dls2/plugin/py_periodic_app.hpp"

#include <stdexcept>

namespace dls
{
PyPeriodicApp::PyPeriodicApp(const std::string& id)
    : dls::PeriodicApp(id)
{
}

PyPeriodicApp::~PyPeriodicApp()
{
    shutdown();
}

void PyPeriodicApp::start()
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

void PyPeriodicApp::activate()
{
    sm.raiseEvent(sm.activation_request);
}

void PyPeriodicApp::deactivate()
{
    sm.raiseEvent(sm.deactivation_request);
}

void PyPeriodicApp::shutdown()
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

void PyPeriodicApp::serve()
{
    start();
    if (app_thread_.joinable())
    {
        app_thread_.join();
    }
}

bool PyPeriodicApp::isStarted() const
{
    return started_.load();
}

std::string PyPeriodicApp::stateName()
{
    return sm.getStateName();
}

bool PyPeriodicApp::checkActivation()
{
    return pythonCheckActivation();
}

bool PyPeriodicApp::pythonCheckActivation()
{
    return true;
}

void PyPeriodicApp::pythonRun()
{
    throw std::runtime_error("Python subclass must override run()");
}

void PyPeriodicApp::run(const std::chrono::system_clock::time_point&)
{
    py::gil_scoped_acquire acquire;
    pythonRun();
}
} // namespace dls
