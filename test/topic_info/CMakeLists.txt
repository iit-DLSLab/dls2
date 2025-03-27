add_executable(ddstopic
	src/ddstopic.cpp
)

target_link_libraries(ddstopic
	dls_com
	dls_messages
	fastdds
	fastcdr
)
dls_install(ddstopic)

# -- create_periodic_plugin
install(
	FILES create_ddstopic_command
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
	DESTINATION	${DLS_INSTALL_LAUNCH_RUNTIME_DIR}
	RENAME "ddstopic"
	COMPONENT dls_dev
)





