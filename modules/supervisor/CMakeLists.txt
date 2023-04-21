add_library(dls_supervisor
	SHARED
		src/supervisor.cpp
)

set_property(TARGET dls_supervisor PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_supervisor
	PUBLIC
		include
)

target_link_libraries(dls_supervisor
	PRIVATE
		dls_com
		dls_command
		dls_topics
		dls_domains
		dls_logging
)

dls_install(dls_supervisor)
