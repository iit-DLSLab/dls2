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
		dls_message_wrappers
		#${MSGS_WRAPPERS_LIBRARY_NAME}
		${TOPICS_LIBRARY_NAME}
		yaml
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
	COMPONENT   ${ACTION_NAME}_dev
)
install(
	DIRECTORY config
	DESTINATION ${DLS_ACTIONS_HEADER_DIR}/${ACTION_NAME}
	COMPONENT   ${ACTION_NAME}_dev
)
install(
	FILES 				config/scheduler.yaml
	DESTINATION 		${DLS_SCHEDULERS_CONFIG_DIR}/${ACTION_NAME}
	COMPONENT   		${ACTION_NAME}
)
install(
	TARGETS             ${ACTION_NAME}
	LIBRARY DESTINATION ${DLS_INSTALL_ACTION_DIR}/${ACTION_NAME}
	COMPONENT           ${ACTION_NAME}
)