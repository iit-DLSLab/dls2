add_library(
	dls_debug
	SHARED
	debug/debug.cpp
)
dls_install(dls_debug)

add_library(
	dls_logging
	SHARED
	log/log.cpp
)

target_link_libraries(dls_logging
	PRIVATE
		fastcdr
		fastrtps
)
dls_install(dls_logging)

add_library(dls_time
	SHARED
	time/time.cpp
)
dls_install(dls_time)

# ==============================================================================
# Tools
# ==============================================================================
add_executable(topic_info
	EXCLUDE_FROM_ALL
	${CMAKE_SOURCE_DIR}/src/util/topic_info/topic_info.cpp
	${CMAKE_SOURCE_DIR}/src/util/topic_info/topic_info_main.cpp

	# TODO this should be put into the dls_messaging library and just linked
	# into this executable. It should NOT be recompiled here, it should only be
	# compiled in the library
	${CMAKE_SOURCE_DIR}/src/controller/control_signal.cpp
)
target_link_libraries(topic_info
	fastcdr
	fastrtps
	dls_messaging
	dls_debug

	# TODO this should be removed when the gait signal and contro lsignal
	# wrappers are moved to the messaging library
	# dls_gait_generator
	# dls_controlinterface
	# dls_estimator
	# dls_framework
)
