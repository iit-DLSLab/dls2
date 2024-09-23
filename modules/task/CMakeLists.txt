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

dls_install(task)