add_executable(topic_info
	EXCLUDE_FROM_ALL
		src/main.cpp
		src/topic_info.cpp
)

target_link_libraries(topic_info
	PRIVATE
		dls_com
		dls_debug
)


add_executable(ddstopic
	src/ddstopic.cpp
)

target_link_libraries(ddstopic
	dls_framework
	dls_messages
	fastrtps
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
)





