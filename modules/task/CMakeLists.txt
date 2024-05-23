add_library(task SHARED
	src/task.cpp
)

set_property(TARGET task PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(task
	PUBLIC
		include
)

target_link_libraries(task
	PUBLIC
		periodic_app_plugin
		state_machine_watcher
)

install(
	DIRECTORY   include/
	DESTINATION ${DLS_DEFAULT_HEADER_DIR}
	COMPONENT   task_dev
)
install(
	TARGETS             task
	LIBRARY DESTINATION ${DLS_INSTALL_LIB_DIR}
	COMPONENT           task
)