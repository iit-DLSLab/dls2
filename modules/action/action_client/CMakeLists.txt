add_library(dls_action_client SHARED
	src/action_client_base.cpp)
set_property(TARGET dls_action_client PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(dls_action_client
	PUBLIC
		include
)
target_link_libraries(dls_action_client
	PUBLIC
		dls_com
		dls_command
		dls_topics
		dls_domains
)

dls_install(dls_action_client)