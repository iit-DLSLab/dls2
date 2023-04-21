add_library(dls_controller SHARED
	src/controller.cpp
    src/controller_data.cpp
)

set_property(TARGET dls_controller PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_controller
	PUBLIC
		include
)

target_link_libraries(dls_controller
	PRIVATE
		robotlib
		dls_math
		dls_app
		dls_command
		dls_logging
		dls_message_wrappers
		dls_com
		dls_topics
		dls_domains
)

dls_install(dls_controller)
