add_library(dls_framework_components
	src/app_layer_component.cpp
	src/periodic_app_layer_component.cpp
)

set_property(TARGET dls_framework_components PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_framework_components
	PUBLIC
		include
)

target_link_libraries(dls_framework_components
	PUBLIC
		dls_command
		dls_logging
		dls_time
)

dls_install(dls_framework_components)
