dls_configure(src/control_layer.cpp.in CONTROL_LAYER_CPP)
dls_configure(src/version_info.cpp.in  VERSION_INFO_CPP)
dls_configure(src/console_layer.cpp.in CONSOLE_LAYER_CPP)
dls_configure(src/app_layer.cpp.in     APP_LAYER_CPP)

add_library(dls_framework SHARED
	src/hyq_app.cpp
	${APP_LAYER_CPP}
	src/hardware_layer.cpp
	src/hyq_app_libraries.cpp
	${CONTROL_LAYER_CPP}
	${VERSION_INFO_CPP}
	${CONSOLE_LAYER_CPP}
	src/estimation_layer.cpp
	src/log_layer.cpp
	src/init.cpp
	src/options.cpp
)

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
		dls_path_prefixes
		dls_controller
		dls_estimator
		dls_gait_generator
		dl
		dls_class_loader
)

add_dependencies(dls_framework
	dls_dls2_messaging
)

dls_install(dls_framework)
