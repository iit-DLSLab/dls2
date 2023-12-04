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
		dls_app
		dls_message_wrappers
)

dls_install(periodic_plugin_base)