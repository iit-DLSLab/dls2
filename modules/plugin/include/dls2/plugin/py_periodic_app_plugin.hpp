#ifndef PY_PERIODIC_APP_PLUGIN_HPP
#define PY_PERIODIC_APP_PLUGIN_HPP

#include <atomic>
#include <string>
#include <thread>

#include <pybind11/pybind11.h>

#include "dls2/plugin/periodic_app_plugin.hpp"

namespace py = pybind11;

namespace dls
{
class PyPeriodicAppPlugin : public dls::PeriodicAppPlugin
{
public:
    explicit PyPeriodicAppPlugin(const std::string& id);
    ~PyPeriodicAppPlugin() override;

    void start();
    void activate();
    void deactivate();
    void shutdown();
    void serve();

    bool isStarted() const;
    std::string stateName();

    bool checkActivation() override;
    void run(const std::chrono::system_clock::time_point& time) final;

protected:
    virtual bool pythonCheckActivation();
    virtual py::object computeCommand(const py::object& input_msg) = 0;
    virtual py::object makePythonInput() const = 0;
    virtual void applyPythonOutput(const py::object& output_msg) = 0;

    std::thread app_thread_;
    std::atomic<bool> started_{false};
    std::atomic<bool> shutdown_requested_{false};
};
} // namespace dls

#endif // PY_PERIODIC_APP_PLUGIN_HPP
