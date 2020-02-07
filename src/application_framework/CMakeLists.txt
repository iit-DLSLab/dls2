dls_configure(control_layer.cpp.in CONTROL_LAYER_CPP)
dls_configure(version_info.cpp.in VERSION_INFO_CPP)

add_library(dls_framework

	SHARED
	# Framework Classes
	hyq_app.cpp
	app_layer.cpp
	hardware_layer.cpp
	hyq_app_libraries.cpp
	${CONTROL_LAYER_CPP}
	${VERSION_INFO_CPP}
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
dls_install(dls_framework)
