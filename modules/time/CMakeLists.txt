add_library(dls_time SHARED
	src/time.cpp
)
target_include_directories(dls_time PUBLIC include)
add_dependencies(dls_time dls_dls2_messaging)
target_link_libraries(dls_time
	PUBLIC
	dls_topics
	dls_dls2_messaging
)
dls_install(dls_time)
