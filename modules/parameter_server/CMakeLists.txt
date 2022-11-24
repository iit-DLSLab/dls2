# ==============================================================================
# Parameter Server Library
# ==============================================================================
add_library(dls_parameter_server
	SHARED
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
	PUBLIC
		dls_com
		fastcdr
		fastrtps
		dls_topics
		dls_service
		dls_domains
)

dls_install(dls_parameter_server)