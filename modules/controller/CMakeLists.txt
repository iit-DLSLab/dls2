add_library(dls_controller SHARED
	src/controller.cpp
	src/control_signal.cpp
    src/controller_data.cpp
)

set_property(TARGET dls_controller PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_controller
	PUBLIC
		include
)

target_link_libraries(dls_controller
	PUBLIC
		robotlib
		dls_math
		dls_framework_components
		dls_gait_signal
		dls_geometry
		dls_message_wrappers
)

dls_install(dls_controller)
