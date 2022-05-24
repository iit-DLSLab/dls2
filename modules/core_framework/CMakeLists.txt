dls_configure(src/control_layer.cpp.in CONTROL_LAYER_CPP)
dls_configure(src/version_info.cpp.in  VERSION_INFO_CPP)
dls_configure(src/console_layer.cpp.in CONSOLE_LAYER_CPP)
dls_configure(src/ros_layer.cpp.in 	   ROS_LAYER_CPP)
dls_configure(src/app_layer.cpp.in     APP_LAYER_CPP)

add_library(dls_framework
	SHARED
	src/hyq_app.cpp
	${APP_LAYER_CPP}
	src/hardware_layer.cpp
	src/hyq_app_libraries.cpp
	${CONTROL_LAYER_CPP}
	${VERSION_INFO_CPP}
	${CONSOLE_LAYER_CPP}
	${ROS_LAYER_CPP}
	src/estimation_layer.cpp
	src/log_layer.cpp
	src/options.cpp
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
		dls_math
		pthread
		dls_controller
		dls_estimator
		dls_gait_generator
		dls_domains
		dls_topics
		dls_class_loader
		fastcdr
		fastrtps
)

dls_install(dls_framework)

add_subdirectory(test)
