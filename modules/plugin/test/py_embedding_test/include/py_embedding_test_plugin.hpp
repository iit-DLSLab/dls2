#pragma once

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include "dls2/plugin/py_periodic_app_plugin.hpp"

class PyEmbeddingTestPlugin : public dls::PyPeriodicAppPlugin
{
    public:

        PyEmbeddingTestPlugin(const std::string &module_name, 
                              const std::string &module_path, 
                              const std::string &class_name, 
                              const std::string &callback_name, 
                              const std::string &ID, 
                              const std::string &robot_name);

        void run(const std::chrono::system_clock::time_point &time) override;
    
    private:

        std::vector<double> py_output_{3.0, 4.0, 5.0};
        py::array_t<double> out_np_;

        std::vector<double> v1_{1.0, 2.0, 3.0};
        py::array_t<double> np_v1_;

};