add_library(dls_motion_generator
	src/motion_generator.cpp
)

set_property(TARGET dls_motion_generator PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_motion_generator
	PUBLIC
		include
)


target_link_libraries(dls_motion_generator
	PUBLIC
		robotlib
		dls_app
		dls_command
		dls_logging
		dls_com
		dls_topics
		dls_domains
		dls_message_wrappers
        yaml-cpp
)

dls_install(dls_motion_generator)
