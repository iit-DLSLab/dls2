set (EXECUTABLE_NAME py_embedding_test)
set (PLUGIN_NAME py_embedding_test_plugin)

add_compile_options("-std=c++2a")

add_library(${PLUGIN_NAME} SHARED
	src/py_embedding_test_plugin.cpp
)

set(PYBIND11_FINDPYTHON ON)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

target_link_libraries(${PLUGIN_NAME}
	PUBLIC
	dls_com
	dls_signal
	dls_messages
	dls_topics
	py_periodic_app_plugin
	Python3::Python
	pybind11::pybind11
)
target_include_directories(${PLUGIN_NAME}
	PUBLIC
		include
		${Python3_INCLUDE_DIRS}
)

# Define executable for running the plugin as an application, without DLS2 console
add_executable(${EXECUTABLE_NAME}
    src/main.cpp
)

target_link_libraries(${EXECUTABLE_NAME}
    ${PLUGIN_NAME}
)

install(
	FILES 				config/scheduler.yaml
	DESTINATION 		${DLS_SCHEDULERS_CONFIG_DIR}
	COMPONENT   		dls_runtime
	RENAME 				${PLUGIN_NAME}.yaml
)

# installing the library to make it loadable at runtime through DLS2 console
install(
	TARGETS             ${PLUGIN_NAME}
	LIBRARY DESTINATION ${DLS_INSTALL_PERIODIC_APP_PLUGIN_DIR}
	COMPONENT  ${PROJECT_NAME}
)
