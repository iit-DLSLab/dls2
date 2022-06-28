# ==============================================================================
# Parameter Server Library
# ==============================================================================
add_library(dls_parameter_server
	src/parameter_server.cpp
	src/parameter_server_client.cpp
)

target_include_directories(dls_parameter_server
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(dls_parameter_server
	PRIVATE
		parameter-store 
		dls_pubsub
		fastcdr
		fastrtps
		dls_topics
		dls_service
		dls_domains
)

# ==============================================================================
# CLI Interface to Parameter Server
# ==============================================================================
add_executable(dls_parameter_server_cli
	src/parameter_server_cli.cpp
)

target_link_libraries(dls_parameter_server_cli
	PRIVATE
		dls_parameter_server
		dls_pubsub
		dls-messages
		fastcdr
		fastrtps
		dls_domains
		dls_topics
)

target_include_directories(dls_parameter_server_cli
	PRIVATE
		include
		src
)

dls_install(dls_parameter_server)
