add_library(dls_time
	src/time.cpp
)

set_property(TARGET dls_time PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_time
	PUBLIC
		include
)

target_link_libraries(dls_time
	PUBLIC
		dls_pubsub
)

dls_install(dls_time)