add_library(dls_pubsub_impl STATIC
		src/participant.cpp
		src/subscriber_base.cpp
)

target_include_directories(dls_pubsub_impl
	PUBLIC
		include
)

target_link_libraries(dls_pubsub_impl
	PRIVATE
		fastcdr
		fastrtps
)

set_property(TARGET dls_pubsub_impl PROPERTY POSITION_INDEPENDENT_CODE TRUE)

dls_install(dls_pubsub_impl)

#add_subdirectory(test)
