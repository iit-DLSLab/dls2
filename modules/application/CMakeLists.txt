add_compile_options("-std=c++2a")
set(DEBUG_SCHEDULER OFF CACHE BOOL "Enable run time collection of periodic application components")
if (DEBUG_SCHEDULER)
	add_definitions(-DDEBUG_SCHEDULER=1)
endif(DEBUG_SCHEDULER)

dls_configure(src/layer.cpp.in  LAYER_CPP)

add_library(dls_app
	SHARED
		src/sched_attr.cpp
		src/app_data.cpp
		src/app.cpp
		${LAYER_CPP}
		src/periodic_app.cpp
		src/process_resource_monitor.cpp
		src/state_machine/initialization.cpp
		# src/state_machine/breaking_rt.cpp
		src/state_machine/run.cpp
		src/state_machine/quit.cpp
		src/state_machine/idle.cpp
		src/state_machine/fail.cpp
		src/state_machine/activation.cpp
		src/state_machine/deactivation.cpp
		src/state_machine/app_state_machine.cpp
		src/state_machine/app_state.cpp
)

set_property(TARGET dls_app PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_app
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(dls_app
	PUBLIC
		yaml-cpp
		dls_com
		dls_time
		dls_command
		dls_logging
		dls_class_loader
		dls_topics
		dls_domains
		state_machine
)

dls_install(dls_app)