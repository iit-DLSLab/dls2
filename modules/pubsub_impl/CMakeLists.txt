add_library(dls_pubsub_impl STATIC
		src/dds_participant.cpp
		src/dds_listeners.cpp
		src/dds_reader.cpp
		src/dds_writer.cpp
)

target_include_directories(dls_pubsub_impl
	PUBLIC
		include
)

target_link_libraries(dls_pubsub_impl
	PRIVATE
		# fastcdr
		# fastrtps
		dls_topics
		dls_domains
)

set_property(TARGET dls_pubsub_impl PROPERTY POSITION_INDEPENDENT_CODE TRUE)

dls_install(dls_pubsub_impl)

#add_subdirectory(test)
