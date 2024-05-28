set(PROCEDURE_NAME stand_up)

add_library(${PROCEDURE_NAME} SHARED
	src/${PROCEDURE_NAME}.cpp
)

set_property(TARGET ${PROCEDURE_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(${PROCEDURE_NAME}
	PUBLIC
		include
)

target_link_libraries(${PROCEDURE_NAME}
	PUBLIC
		app_plugin
		dls_utils
		state_machine_watcher
)

install(
	DIRECTORY   include/
	DESTINATION ${DLS_PROCEDURES_HEADER_DIR}
	COMPONENT   ${PROCEDURE_NAME}_dev
)
install(
	TARGETS             ${PROCEDURE_NAME}
	LIBRARY DESTINATION ${DLS_INSTALL_PROCEDURE_DIR}
	COMPONENT           ${PROCEDURE_NAME}
)