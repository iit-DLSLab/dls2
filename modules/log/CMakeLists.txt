add_library(dls_logging
	src/log.cpp
)

target_include_directories(dls_logging
	PUBLIC
		include
)

target_link_libraries(dls_logging
	PUBLIC
		dls_pubsub
)

dls_install(dls_logging)
