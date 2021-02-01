add_library(dls_controller
	SHARED
		src/controller.cpp
		src/control_signal.cpp
)

target_include_directories(dls_controller
	PUBLIC
		include
)

target_link_libraries(dls_controller
	PUBLIC
		dls_framework_components
		dls_gait_signal
)

dls_install(dls_controller)
