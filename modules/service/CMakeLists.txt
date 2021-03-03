add_library(dls_service INTERFACE)

target_include_directories(dls_service
	INTERFACE
		include
)

target_link_libraries(dls_service
	INTERFACE
		dls_pubsub_impl
)

dls_install(dls_service)
