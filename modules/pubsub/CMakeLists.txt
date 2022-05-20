add_library(dls_pubsub INTERFACE)

target_link_libraries(dls_pubsub
	INTERFACE
		dls_domains
		dls_service
		dls_dls2_messaging
		dls_message_wrappers
		dls_topics
)

dls_install(dls_pubsub)
