configure_file(
	control_layer.cpp.in
	${CMAKE_BINARY_DIR}/src/application_framework/control_layer.cpp
)

configure_file(
	version_info.cpp.in
	${CMAKE_BINARY_DIR}/src/application_framework/version_info.cpp
)
add_library(dls_framework

	SHARED
	# Framework Classes
	hyq_app.cpp
	app_layer.cpp
	hardware_layer.cpp
	hyq_app_libraries.cpp
	${CMAKE_BINARY_DIR}/src/application_framework/control_layer.cpp
	${CMAKE_BINARY_DIR}/src/application_framework/version_info.cpp
	console_layer.cpp
	estimation_layer.cpp
	log_layer.cpp

	options.cpp

	# Components
	components/app_layer_component.cpp
	components/periodic_app_layer_component.cpp


	# TODO remove this and instead link to the library
	${CMAKE_SOURCE_DIR}/src/util/string/string.cpp
)
target_link_libraries(dls_framework
	PRIVATE
		readline # for console_layer
		fastcdr
		fastrtps
		dls_time

	PUBLIC # TODO fix this
	#dls_string_util TODO link to this library and remove the source above
		dls_logging
		dls_messaging
		pthread
		dl
		dls_debug
		dls_command
)
# list(APPEND PROJECT_TARGETS dls_framework)
dls_install(dls_framework)
