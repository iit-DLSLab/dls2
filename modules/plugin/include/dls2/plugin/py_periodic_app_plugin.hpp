#pragma once

#include <dls2/plugin/periodic_app_plugin.hpp>

#include <pybind11/embed.h>

namespace py = pybind11;

namespace dls
{
    class PyPeriodicAppPlugin : public dls::PeriodicAppPlugin
	{
		public:
			PyPeriodicAppPlugin(
				const std::string &module_name, 
				const std::string &module_path, 
				const std::string &class_name, 
				const std::string &callback_name, 
				const std::string &ID, 
				const std::string &robot_name);
			~PyPeriodicAppPlugin();

			/**
			 * @brief Create Python class object by forwarding arbitrary C++ positional args (pybind11 will convert them to Python values).
			 * 
			 * @tparam Args 
			 * @param args 
			 */
			template <typename... Args>
			void createClassObject(Args&&... args);

			/**
			 * @brief Create a class object by using Python-side args/kwargs if preferred
			 * 
			 * @param args 
			 * @param kwargs 
			 */
			void createClassObject(py::args args, py::kwargs kwargs);

			/**
			 * @brief Generalized callback wrapper forwarding arbitrary C++ args to the Python callback. 
			 * The Python callback is cached at class object creation for efficiency.
			 * 
			 * @tparam ProcArgs 
			 * @param pargs 
			 * @return py::object 
			 */
			template <typename... ProcArgs>
			void callCallback(ProcArgs&&... pargs);

			bool hasInstance() const;

		private:

			struct PyContext{
				py::scoped_interpreter py_guard_{};
				py::object py_class_{ py::none() };
				py::object py_class_object_{ py::none() };
				py::object py_callback_{ py::none() };
			};
			std::unique_ptr<PyContext> py_context_;

			std::string callback_name_{""};
	};
} // end namespace dls

#include "dls2/plugin/py_periodic_app_plugin.tpp"