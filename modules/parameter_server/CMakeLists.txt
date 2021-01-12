# ==============================================================================
# Messages
# ==============================================================================
add_subdirectory(msg)

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
hsrc
)

target_link_libraries(dls_parameter_server
	PRIVATE
	parameter_store
	dls_framework
	param_server_msgs
	fastcdr
	fastrtps
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
)

target_include_directories(dls_parameter_server_cli
	PRIVATE
	include
	src
)
