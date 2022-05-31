add_library(dls_topics SHARED
	src/topics.cpp
)

target_include_directories(dls_topics
	PUBLIC
		include
)

target_link_libraries(dls_topics
	PUBLIC
		dls_messaging
)

dls_install(dls_topics)
