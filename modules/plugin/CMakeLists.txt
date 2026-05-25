add_library(plugin
	SHARED
		src/plugin.cpp
)

target_include_directories(plugin
	PUBLIC
		include
)

target_link_libraries(plugin
	PUBLIC
		dls_app
		dls_signal
        dls_msg_wrapper
)

add_library(periodic_app_plugin
	SHARED
		src/periodic_app_plugin.cpp
		src/topic_monitor.cpp
)

target_include_directories(periodic_app_plugin
	PUBLIC
		include
)

target_link_libraries(periodic_app_plugin
	PUBLIC
		dls_app
		dls_signal
	dls_time
        dls_msg_wrapper
		plugin
	PRIVATE
)

find_package(pybind11 REQUIRED)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

execute_process(
	COMMAND
		${Python3_EXECUTABLE} -c "import sysconfig; schemes = sysconfig.get_scheme_names(); scheme = 'deb_system' if 'deb_system' in schemes else sysconfig.get_default_scheme(); print(sysconfig.get_path('purelib', scheme=scheme))"
	OUTPUT_VARIABLE
		_ABS_PYTHON_MODULE_PATH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

get_filename_component(_ABS_PYTHON_MODULE_PATH ${_ABS_PYTHON_MODULE_PATH} ABSOLUTE)
file(RELATIVE_PATH PYTHON_MODULE_PATH ${CMAKE_INSTALL_PREFIX} ${_ABS_PYTHON_MODULE_PATH})

add_library(py_periodic_app
	STATIC
		src/py_periodic_app.cpp
)

set_target_properties(py_periodic_app PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)

target_include_directories(py_periodic_app
	PUBLIC
		include
)

target_link_libraries(py_periodic_app
	PUBLIC
		dls_app
		dls_time
		pybind11::module
)

pybind11_add_module(dls2_periodic_app_bindings
	src/py_periodic_app_bindings.cpp
)

target_include_directories(dls2_periodic_app_bindings
	PUBLIC
		include
)

target_link_libraries(dls2_periodic_app_bindings
	PRIVATE
		py_periodic_app
)

add_library(app_plugin
	SHARED
		src/app_plugin.cpp
)

target_include_directories(app_plugin
	PUBLIC
		include
)

target_link_libraries(app_plugin
	PUBLIC
		dls_app
		dls_signal
        dls_msg_wrapper
		plugin
)

add_subdirectory(test)
# INSTALL
# -- plugin
dls_install(plugin)
# -- app_plugin
dls_install(app_plugin)
# -- periodic_app_plugin
dls_install(periodic_app_plugin)

# -- python periodic app bindings
install(
	TARGETS dls2_periodic_app_bindings
	LIBRARY
		DESTINATION ${PYTHON_MODULE_PATH}
		COMPONENT dls_runtime
)

install(
	DIRECTORY python/dls2_py
	DESTINATION ${PYTHON_MODULE_PATH}
	COMPONENT dls_runtime
)

install(
	FILES python/config/dds_participant_config.xml
	DESTINATION ${PYTHON_MODULE_PATH}/dls2_py
	COMPONENT dls_runtime
)

# -- skeletons
install(
	DIRECTORY
		skeletons/
	DESTINATION
		/usr/include/dls2/plugin_skeletons
)
# -- create_periodic_plugin
install(
	FILES skeletons/periodic/create_periodic_plugin
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
	DESTINATION	${DLS_INSTALL_LAUNCH_RUNTIME_DIR}
	COMPONENT	dls_runtime
)
