add_library(dls_topics SHARED
	src/topics.cpp
)

# include_directories(build/dls2/msg/gen/include)

target_include_directories(dls_topics
	PUBLIC
		include
)

target_link_libraries(dls_topics
	PUBLIC
		dls_dls2_messaging
)

dls_install(dls_topics)
