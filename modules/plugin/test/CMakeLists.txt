set (EXECUTABLE_NAME plugin_test)
add_executable(${EXECUTABLE_NAME}
    src/main.cpp
	src/custom_plugin.cpp
)

target_link_libraries(${EXECUTABLE_NAME}
    dls_com
	dls_signal
	dls_messages
	dls_topics
	periodic_app_plugin
)
target_include_directories(${EXECUTABLE_NAME}
	PUBLIC
	include
)

install(
	FILES 				config/scheduler.yaml
	DESTINATION 		${DLS_SCHEDULERS_CONFIG_DIR}/${EXECUTABLE_NAME}
	COMPONENT   		dls_runtime
)