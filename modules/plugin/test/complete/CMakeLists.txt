set (EXECUTABLE_NAME custom_plugin_test)
set (PLUGIN_NAME custom_plugin)

add_compile_options("-std=c++2a")

add_library(${PLUGIN_NAME} SHARED
	src/custom_plugin.cpp
)

target_link_libraries(${PLUGIN_NAME}
	dls_com
	dls_signal
	dls_messages
	dls_topics
	periodic_app_plugin
)
target_include_directories(${PLUGIN_NAME}
	PUBLIC
	include
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
	DESTINATION 		${DLS_SCHEDULERS_CONFIG_DIR}/${PLUGIN_NAME}
	COMPONENT   		dls_runtime
)

# installing the library to make it loadable at runtime through DLS2 console
install(
	TARGETS             ${PLUGIN_NAME}
	LIBRARY DESTINATION ${DLS_INSTALL_GENERIC_PLUGIN_DIR}
	COMPONENT  ${PROJECT_NAME}
)
