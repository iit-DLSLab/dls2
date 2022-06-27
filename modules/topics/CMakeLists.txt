add_library(dls_topics SHARED
	src/topics.cpp
)

target_include_directories(dls_topics
	PUBLIC
		include
		${CMAKE_CURRENT_BINARY_DIR}/dls-messages/include
)

target_link_libraries(dls_topics
	PUBLIC
		dls-messages
)

dls_install(dls_topics)
