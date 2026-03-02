#pragma once

#include <dls2/plugin/periodic_app_plugin.hpp>

namespace dls
{
	template <typename... Args>
	void PyPeriodicAppPlugin::createClassObject(Args&&... args) 
	{
		py::gil_scoped_acquire gil;
		try 
		{
			py_class_object_ = py_class_(std::forward<Args>(args)...);

			if(py::hasattr(py_class_object_, callback_name_.c_str())){
				py_callback_ = py_class_object_.attr(callback_name_.c_str());
			}
			else{
				throw std::runtime_error("The specified callback method '" + callback_name_ + "' does not exist in the Python class.");
			}
		} catch (const py::error_already_set &e) 
		{
			throw std::runtime_error(std::string("Failed to instantiate Logic: ") + e.what());
		}
	}

	template <typename... ProcArgs>
	py::object PyPeriodicAppPlugin::callCallback(ProcArgs&&... pargs) 
	{
		py::gil_scoped_acquire gil;
		if (py_class_object_.is_none())
		{
			throw std::runtime_error("py_class_object_ instance not created");
		}
		if (py_callback_.is_none())
		{
			throw std::runtime_error("py_callback_ not initialized");
		}
		try 
		{
			return py_callback_(std::forward<ProcArgs>(pargs)...);
		} catch (const py::error_already_set &e) 
		{
			throw std::runtime_error(std::string("Python error in process(): ") + e.what());
		}
	}
    
} // end namespace dls