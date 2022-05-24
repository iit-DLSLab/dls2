add_library(dls_pubsub SHARED
		src/dds_participant.cpp
		src/dds_listeners.cpp
		src/dds_reader.cpp
		src/dds_writer.cpp
)

target_include_directories(dls_pubsub
	PUBLIC
		include
)

target_link_libraries(dls_pubsub
	PRIVATE
		fastcdr
		fastrtps
		dls_domains
		dls_topics
)

set_property(TARGET dls_pubsub PROPERTY POSITION_INDEPENDENT_CODE TRUE)

dls_install(dls_pubsub)

#add_subdirectory(test)
