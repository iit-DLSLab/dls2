add_subdirectory(msg)
add_library(dls_parameter_server
	src/parameter_server.cpp
)

target_include_directories(dls_parameter_server
	PUBLIC
	include
)

target_link_libraries(dls_parameter_server
	PRIVATE
	parameter_store
	dls_framework
	param_server_msgs
)
