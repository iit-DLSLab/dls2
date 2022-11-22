add_compile_options("-std=c++17")

dls_configure(src/version_info.cpp.in  VERSION_INFO_CPP)
dls_configure(src/app_layer.cpp.in  APP_LAYER_CPP)
dls_configure(src/control_layer.cpp.in  CONTROL_LAYER_CPP)

add_library(dls_framework
	SHARED
	src/hardware_layer.cpp
	src/console_layer.cpp
	src/sim_layer.cpp
	src/estimation_layer.cpp
	src/log_layer.cpp
	src/options.cpp
	${CONTROL_LAYER_CPP}
    ${VERSION_INFO_CPP}
    ${APP_LAYER_CPP}
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
		dls_controller
		dls_estimator
        dls_hardware
		dls_gait_generator
		dls_domains
		dls_topics
		dls_class_loader
		dls_foxglove
		fastcdr
		fastrtps
)

dls_install(dls_framework)

add_subdirectory(test)
