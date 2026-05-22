#include "dls2/plugin/py_periodic_app.hpp"

#include <memory>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace
{
class PyPeriodicAppTrampoline : public dls::PyPeriodicApp
{
public:
    using dls::PyPeriodicApp::PyPeriodicApp;

protected:
    bool pythonCheckActivation() override
    {
        py::gil_scoped_acquire acquire;
        py::function override = py::get_override(
            static_cast<const dls::PyPeriodicApp*>(this),
            "check_activation");
        if (!override)
        {
            return dls::PyPeriodicApp::pythonCheckActivation();
        }
        return override().cast<bool>();
    }

    void pythonRun() override
    {
        py::gil_scoped_acquire acquire;
        py::function override = py::get_override(
            static_cast<const dls::PyPeriodicApp*>(this),
            "run");
        if (!override)
        {
            dls::PyPeriodicApp::pythonRun();
            return;
        }
        override();
    }
};
} // namespace

PYBIND11_MODULE(dls2_periodic_app_bindings, m)
{
    py::class_<
        dls::PyPeriodicApp,
        PyPeriodicAppTrampoline,
        std::shared_ptr<dls::PyPeriodicApp>>(m, "PeriodicApp")
        .def(py::init<const std::string&>())
        .def("start", &dls::PyPeriodicApp::start)
        .def("serve", [](dls::PyPeriodicApp& self) {
            py::gil_scoped_release release;
            self.serve();
        })
        .def("activate", &dls::PyPeriodicApp::activate)
        .def("deactivate", &dls::PyPeriodicApp::deactivate)
        .def("shutdown", [](dls::PyPeriodicApp& self) {
            py::gil_scoped_release release;
            self.shutdown();
        })
        .def("state", &dls::PyPeriodicApp::stateName)
        .def("is_started", &dls::PyPeriodicApp::isStarted);
}
