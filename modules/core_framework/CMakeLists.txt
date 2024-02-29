add_compile_options("-std=c++17")

dls_configure(src/version_info.cpp.in  VERSION_INFO_CPP)
dls_configure(src/control_layer.cpp.in  CONTROL_LAYER_CPP)
dls_configure(src/console_layer.cpp.in  CONSOLE_LAYER_CPP)
dls_configure(src/hardware_layer.cpp.in  HARDWARE_LAYER_CPP)
dls_configure(src/options.cpp.in  OPTIONS_CPP)

add_library(dls_framework
	SHARED
		${CONSOLE_LAYER_CPP}
		${CONTROL_LAYER_CPP}
		src/estimation_layer.cpp
		${HARDWARE_LAYER_CPP}
		src/log_layer.cpp
		${OPTIONS_CPP}
		src/service_layer.cpp
		${VERSION_INFO_CPP}
		src/core_interface.cpp
)

set_property(TARGET dls_framework PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_framework
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(dls_framework
	PRIVATE
		readline # for console_layer

	PUBLIC
        robotlib
		dls_math
		pthread
		dls_com
		dls_app
		dls_controller
		dls_command
		dls_message_wrappers
		dls_estimator
        dls_hardware
		dls_service
		dls_motion_generator
		dls_domains
		dls_topics
		dls_class_loader
		dls_mcap
		dls_time
		dls_messages
)

dls_install(dls_framework)

add_subdirectory(test)
