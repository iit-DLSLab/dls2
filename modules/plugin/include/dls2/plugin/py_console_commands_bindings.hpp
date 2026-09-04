#include <pybind11/pybind11.h>
#include <functional>
#include <memory>

namespace py = pybind11;

struct PythonCommandBridge {
    py::function fn;

    explicit PythonCommandBridge(py::function f)
        : fn(std::move(f)) {}

    bool call_noarg() {
        py::gil_scoped_acquire acquire;
        py::object ret = fn();
        return ret.cast<bool>();
    }

    bool call_string(const std::string& s) {
        py::gil_scoped_acquire acquire;
        py::object ret = fn(s);
        return ret.cast<bool>();
    }
};