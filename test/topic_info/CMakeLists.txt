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

add_executable(dls_hz
	src/message_hz.cpp
)

target_link_libraries(dls_hz
	dls_framework
	dls_messages
	fastrtps
	fastcdr
)
dls_install(dls_hz)


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





