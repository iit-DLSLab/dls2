add_library(dls_action_server_base INTERFACE)
set_property(TARGET dls_action_server_base PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(dls_action_server_base
	INTERFACE
		include
)
target_link_libraries(dls_action_server_base
	INTERFACE
		dls_com
		dls_app
		dls_command
		dls_topics
		dls_domains
		dls_logging
		periodic_plugin_base
)

dls_install(dls_action_server_base)