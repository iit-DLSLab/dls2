add_library(dls_topics SHARED
	src/topics.cpp
)

target_include_directories(dls_topics
	PUBLIC
		include
		${CMAKE_CURRENT_BINARY_DIR}/dls_messages/include
)

target_link_libraries(dls_topics
	PRIVATE
		dls_messages
)

dls_install(dls_topics)
