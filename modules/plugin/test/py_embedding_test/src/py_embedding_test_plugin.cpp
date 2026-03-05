#include <iostream>
#include <string>

#include "py_embedding_test_plugin.hpp"

PyEmbeddingTestPlugin::PyEmbeddingTestPlugin(const std::string &module_name, 
                                             const std::string &module_path, 
                                             const std::string &class_name, 
                                             const std::string &callback_name, 
                                             const std::string &ID, 
                                             const std::string &robot_name)
: dls::PyPeriodicAppPlugin(module_name, module_path, class_name, callback_name, ID, robot_name)
{
    // Instantiate Python class: MyLogic(gain, robot_name)
    double gain = 1.23;
    this->createClassObject(gain, robot_name);
    std::cout << "[C++] Created MyLogic instance via PyPeriodicAppPlugin\n";
}

void PyEmbeddingTestPlugin::run(const std::chrono::system_clock::time_point &time)
{
    try {
        for(auto& i : v1_){
            i++;
        }
        
        {
	        py::gil_scoped_acquire gil;
            np_v1_ = py::array_t<double>(v1_.size(), v1_.data());
            out_np_ = py::array_t<double>(
                {static_cast<ssize_t>(py_output_.size())},      // shape
                {static_cast<ssize_t>(sizeof(double))},  // stride
                py_output_.data(),                              // data pointer
                py::capsule(py_output_.data(), [](void*) {})    // no-op deleter
            );
        }

        // Call Python method MyLogic.tick()
        this->callCallback(np_v1_, out_np_);
        std::cout << "[C++] Called MyLogic.tick()\n";

    } catch (const std::exception& e) {
        std::cerr << time.time_since_epoch().count() << " [C++] Exception: " << e.what() << std::endl;
    }
}