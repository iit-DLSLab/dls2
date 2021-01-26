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

add_dependencies(dls_logging dls_dls2_messaging)
add_dependencies(dls_time dls_dls2_messaging)
