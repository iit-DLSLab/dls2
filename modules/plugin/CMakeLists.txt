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

add_library(py_periodic_app_plugin
	SHARED
		src/py_periodic_app_plugin.cpp
)

target_include_directories(py_periodic_app_plugin
	PUBLIC
		include
)

target_link_libraries(py_periodic_app_plugin
	PUBLIC
		periodic_app_plugin
		pybind11::module
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
# -- py_periodic_app_plugin
dls_install(py_periodic_app_plugin)

install(
	DIRECTORY
		python/dls2_py
	DESTINATION
		/usr/lib/python3/dist-packages
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
