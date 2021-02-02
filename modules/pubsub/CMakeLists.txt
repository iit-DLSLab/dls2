add_library(dls_pubsub STATIC
		src/participant.cpp
)

target_include_directories(dls_pubsub
	PUBLIC
		include
)

target_link_libraries(dls_pubsub
	INTERFACE
		fastcdr
		fastrtps
		dls_topics
		dls_service
		dls_dls2_messaging
		dls_message_wrappers
)

dls_install(dls_pubsub)

set_property(TARGET dls_pubsub PROPERTY POSITION_INDEPENDENT_CODE TRUE)

add_subdirectory(test)
