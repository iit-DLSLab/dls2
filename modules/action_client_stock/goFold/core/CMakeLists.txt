# Create plugin library
add_library(${ACTION_NAME} SHARED
	src/go_fold_server.cpp
	src/go_fold_client.cpp
)

set_property(TARGET ${ACTION_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(${ACTION_NAME}
	PUBLIC
		include
		# add other include directories here
)
target_link_libraries(${ACTION_NAME}
	PUBLIC
		dls_action_server_base
		dls_action_client
		dls_signal
		${TOPICS_LIBRARY_NAME}
		yaml-cpp
		# add other linked libraries here
)
target_link_directories(${ACTION_NAME}
	PUBLIC
        /usr/lib/dls2
		${DLS_INSTALL_ACTION_DIR}/${ACTION_NAME}
)

# ================================== Install =================================

install(
	DIRECTORY   include/
	DESTINATION ${DLS_DEFAULT_HEADER_DIR}
	COMPONENT   dls_dev
)
install(
	DIRECTORY config
	DESTINATION ${DLS_ACTIONS_HEADER_DIR}/${ACTION_NAME}
	COMPONENT   dls_runtime
)
install(
	FILES 				config/scheduler.yaml
	DESTINATION 		${DLS_SCHEDULERS_CONFIG_DIR}/${ACTION_NAME}
	COMPONENT   		dls_runtime
)
install(TARGETS ${ACTION_NAME}
	LIBRARY
		DESTINATION ${DLS_INSTALL_ACTION_DIR}/${ACTION_NAME}
		COMPONENT   dls_runtime
	ARCHIVE
		DESTINATION /usr/lib
		COMPONENT dls_runtime
	RUNTIME
		DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
		COMPONENT dls_runtime
)
