add_executable(topic_info
	EXCLUDE_FROM_ALL
		src/main.cpp
		src/topic_info.cpp
)

target_link_libraries(topic_info
	PRIVATE
		dls_pubsub
		dls_debug
)
