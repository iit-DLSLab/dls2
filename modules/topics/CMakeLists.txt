add_library(dls_topics INTERFACE)

target_include_directories(dls_topics
	INTERFACE
		include
)

target_link_libraries(dls_topics
	INTERFACE
		dls_pubsub
)
