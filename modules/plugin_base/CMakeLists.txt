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

# INSTALL
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