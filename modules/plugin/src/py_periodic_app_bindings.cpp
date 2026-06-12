#include "dls2/application/periodic_app.hpp"

#include <memory>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace
{
class PyPeriodicAppTrampoline : public dls::PeriodicApp
{
public:
    using dls::PeriodicApp::PeriodicApp;

protected:
    bool checkActivation() override
    {
        py::gil_scoped_acquire acquire;
        py::function override = py::get_override(
            static_cast<const dls::App*>(this),
            "check_activation");
        if (!override)
        {
            return true;
        }
        return override().cast<bool>();
    }

    void run(const std::chrono::system_clock::time_point& time) override
    {
        py::gil_scoped_acquire acquire;
        py::function override = py::get_override(
            static_cast<const dls::PeriodicApp*>(this),
            "run");
        if (!override)
        {
            throw std::runtime_error("Python subclass must override run()");
            return;
        }

        const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            time.time_since_epoch()).count();

        override(ns);
    }
};
} // namespace

PYBIND11_MODULE(dls2_periodic_app_bindings, m)
{
    py::class_<
        dls::PeriodicApp,
        PyPeriodicAppTrampoline,
        std::shared_ptr<dls::PeriodicApp>>(m, "PeriodicApp")
        .def(py::init<const std::string&>())
        .def("execute", &dls::App::execute)
        .def("shutdown", [](dls::PeriodicApp& self) {
            py::gil_scoped_release release;
            self.stop();
        });
}
