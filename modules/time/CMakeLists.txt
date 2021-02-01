add_library(dls_time
	src/time.cpp
)

target_include_directories(dls_time
	PUBLIC
		include
)

target_link_libraries(dls_time
	PUBLIC
		dls_pubsub
)


dls_install(dls_time)
