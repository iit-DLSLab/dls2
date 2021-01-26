add_library(dls_logging SHARED
	src/log.cpp
)

target_link_libraries(dls_logging
	PRIVATE
		fastcdr  # TODO use the wrapped library
		fastrtps # TODO use the wrapped library
)
add_dependencies(dls_logging dls_dls2_messaging)

dls_install(dls_logging)
