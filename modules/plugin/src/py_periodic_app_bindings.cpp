#include "dls2/application/periodic_app.hpp"
#include "dls2/plugin/py_console_commands_bindings.hpp"

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

    bool deactivation(const std::chrono::system_clock::time_point& time) override
    {
        py::gil_scoped_acquire acquire;
        py::function override = py::get_override(
            static_cast<const dls::PeriodicApp*>(this),
            "deactivation");
        if (!override)
        {
            return true;
        }

        const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            time.time_since_epoch()).count();

        return override(ns).cast<bool>();
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
        })
        .def("add_command",
            [](dls::PeriodicApp& self,
                const std::string& name,
                const std::string& doc,
                py::function callback,
                bool enabled = false) {
                    auto bridge = std::make_shared<PythonCommandBridge>(std::move(callback));
                    py::object parameters = py::module_::import("inspect")
                                                .attr("signature")(bridge->fn)
                                                .attr("parameters");

                    if (py::len(parameters) == 0) {
                        std::function<bool()> command_fn = [bridge]() {
                            return bridge->call_noarg();
                        };

                        self.command_manager.addCommand<>(
                            name,
                            doc,
                            command_fn,
                            {},
                            enabled
                        );
                        return;
                    }

                    if (py::len(parameters) == 1) {
                        std::function<bool(std::string)> command_fn =
                            [bridge](std::string value) {
                                return bridge->call_string(value);
                            };

                        self.command_manager.addCommand<std::string>(
                            name,
                            doc,
                            command_fn,
                            {},
                            enabled
                        );
                        return;
                    }

                    throw py::type_error(
                        "add_command callback must accept zero arguments or one string argument"
                    );
                },
            py::arg("name"),
            py::arg("doc"),
            py::arg("callback"),
            py::arg("enabled") = false);
}
