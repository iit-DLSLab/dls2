add_library(dls_hardware 
	SHARED
		src/hardware_data.cpp
		src/hardware.cpp
)

set_property(TARGET dls_hardware PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_hardware
	PUBLIC
		include
		/usr/include
)

target_link_libraries(dls_hardware
	PUBLIC
		dls_hal
		robotlib
		dls_app
		dls_command
		dls_logging
		dls_com
		dls_topics
		dls_domains
)

dls_install(dls_hardware)
