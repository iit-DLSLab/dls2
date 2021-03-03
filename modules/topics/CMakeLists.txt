add_library(dls_topics INTERFACE)

target_include_directories(dls_topics
	INTERFACE
		include
)

dls_install(dls_topics)
