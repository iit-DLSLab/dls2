add_library(dls_supervisor
	SHARED
		src/supervisor.cpp
		src/orchestrator_base.cpp
		src/pegasus_orchestrator.cpp
)

set_property(TARGET dls_supervisor PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_supervisor
	PUBLIC
		include
)

target_link_libraries(dls_supervisor
	PUBLIC
		periodic_app_plugin
		state_machine
		dls_time
		dls_app
		dls_utils
		state_machine_watcher
)

add_subdirectory(test)

install(
    FILES
        data/safety_layer.yaml
    DESTINATION
        /usr/include/dls2/supervisor/data
    COMPONENT
        dls_dev
	RENAME 
		safety_layer.yaml
)

dls_install(dls_supervisor)
