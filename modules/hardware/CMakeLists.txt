add_library(dls_hardware SHARED
	src/hardware.cpp
)

set_property(TARGET dls_hardware PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_hardware
	PUBLIC
		include
)

target_link_libraries(dls_hardware
	PUBLIC
		robotlib
		dls_framework_components
		dls_message_wrappers
)

dls_install(dls_hardware)
