message(WARNING "This module is not yet modular as it takes sources from outside")
add_executable(topic_info
	EXCLUDE_FROM_ALL
	src/main.cpp
	src/topic_info.cpp

	# TODO this should be put into the dls_messaging library and just linked
	# into this executable. It should NOT be recompiled here, it should only be
	# compiled in the library
	${PROJECT_SOURCE_DIR}/src/controller/control_signal.cpp
)
target_link_libraries(topic_info
	fastcdr
	fastrtps
	dls_messaging
	dls_debug
)
