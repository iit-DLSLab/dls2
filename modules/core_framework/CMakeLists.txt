dls_configure(src/control_layer.cpp.in CONTROL_LAYER_CPP)
dls_configure(src/version_info.cpp.in  VERSION_INFO_CPP)
dls_configure(src/console_layer.cpp.in CONSOLE_LAYER_CPP)
dls_configure(src/app_layer.cpp.in     APP_LAYER_CPP)

message(WARNING "core framework module is not a module yet -- it is still taking file from elsewhere")
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
	src/components/app_layer_component.cpp
	src/components/periodic_app_layer_component.cpp

	# TODO remove this and instead link to the library
	# ${PROJECT_SOURCE_DIR}/src/util/string/string.cpp
)
target_include_directories(dls_framework
	PUBLIC include
	PRIVATE src
)

target_link_libraries(dls_framework
	PRIVATE
		readline # for console_layer
		fastcdr
		fastrtps
		dls_time
		dls_pubsub
		dls_class_loader
		dls_path_prefixes

	PUBLIC # TODO fix this
	#dls_string_util TODO link to this library and remove the source above
		dls_logging
		dls_messaging
		pthread
		dl
		dls_debug
		dls_command
)
dls_install(dls_framework)

add_dependencies(dls_framework dls_dls2_messaging)
