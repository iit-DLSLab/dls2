
#include "dls2/plugin/py_periodic_app_plugin.hpp"

namespace dls
{

	PyPeriodicAppPlugin::PyPeriodicAppPlugin(
				const std::string &module_name, 
				const std::string &module_path, 
				const std::string &class_name, 
				const std::string &callback_name, 
				const std::string &ID, 
				const std::string &robot_name)
		: PeriodicAppPlugin(ID)
		, py_guard_()
		, py_class_(py::none())
		, py_class_object_(py::none())
		, py_callback_(py::none())
		, callback_name_(callback_name)
	{
	    py::gil_scoped_acquire gil;

		py::module_ sys = py::module_::import("sys");
		if (!module_path.empty()) {
        	sys.attr("path").attr("insert")(0, module_path);
		}

		// Import module and store the class object (not instantiated yet)
		py::module_ py_lib = py::module_::import(module_name.c_str());
		py_class_ = py_lib.attr(class_name.c_str());
	}

	PyPeriodicAppPlugin::~PyPeriodicAppPlugin()
	{
		py::gil_scoped_acquire gil;
		py_class_ = py::none();
		py_class_object_ = py::none();
		py_callback_ = py::none();
	}

	void PyPeriodicAppPlugin::createClassObject(py::args args, py::kwargs kwargs) 
	{
		py::gil_scoped_acquire gil;

		PyObject* res = PyObject_Call(py_class_.ptr(), args.ptr(),
                              kwargs ? kwargs.ptr() : nullptr);
		if (!res) 
			throw py::error_already_set();
		py_class_object_ = py::reinterpret_steal<py::object>(res);
	}

	bool PyPeriodicAppPlugin::hasInstance() const 
	{ 
		return !py_class_object_.is_none(); 
	}

}

