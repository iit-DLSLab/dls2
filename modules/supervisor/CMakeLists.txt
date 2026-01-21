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
		dls_utils
		state_machine_watcher
)

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
