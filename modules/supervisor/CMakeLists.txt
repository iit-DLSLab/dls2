add_library(dls_supervisor
	SHARED
		src/supervisor.cpp
)

set_property(TARGET dls_supervisor PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_supervisor
	PUBLIC
		include
)

target_link_libraries(dls_supervisor
	PUBLIC
		dls_app
		state_machine_watcher
)

dls_install(dls_supervisor)
