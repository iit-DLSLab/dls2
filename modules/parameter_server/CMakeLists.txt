add_library(dls_param_server
	SHARED
		src/parameter_server.cpp
		src/parameter_client.cpp
)

set_property(TARGET dls_param_server PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_param_server
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(dls_param_server
	PUBLIC
	dls_framework
)

dls_install(dls_param_server)