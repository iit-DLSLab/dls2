add_library(periodic_plugin_base
	SHARED
		src/periodic_plugin_base.cpp
)

target_include_directories(periodic_plugin_base
	PUBLIC
		include
)

target_link_libraries(periodic_plugin_base
	PUBLIC
		robotlib
		dls_app
		dls_command
		dls_com
		dls_topics
		dls_domains
		dls_message_wrappers
		dls_logging
)

dls_install(periodic_plugin_base)