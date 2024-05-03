add_library(plugin_base
	SHARED
		src/plugin_base.cpp
)

target_include_directories(plugin_base
	PUBLIC
		include
)

target_link_libraries(plugin_base
	PUBLIC
		dls_app
		dls_message_wrappers
)

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
		plugin_base
)

# INSTALL
# -- plugin_base
dls_install(plugin_base)
# -- periodic_plugin_base
dls_install(periodic_plugin_base)

# -- skeletons
install(
	DIRECTORY
		skeletons/
	DESTINATION
		/usr/include/dls2/plugin_skeletons
)
# -- create_periodic_plugin
install(
	FILES skeletons/periodic/create_periodic_plugin
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
	DESTINATION	${DLS_INSTALL_LAUNCH_RUNTIME_DIR}
	COMPONENT	dls_runtime
)