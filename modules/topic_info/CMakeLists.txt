message(WARNING "This module is not yet modular as it takes sources from outside")
add_executable(topic_info
	EXCLUDE_FROM_ALL
	src/main.cpp
	src/topic_info.cpp
)
target_link_libraries(topic_info
	fastcdr
	fastrtps
	dls_messaging
	dls_debug
)
