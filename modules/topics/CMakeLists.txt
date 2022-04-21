add_library(dls_topics SHARED
	src/topics.cpp
)

target_include_directories(dls_topics
	PUBLIC
		include
)

dls_install(dls_topics)
