add_library(dls_service 
	SHARED
		src/service_base.cpp
)

set_property(TARGET dls_service PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_service
	PUBLIC
		include
)

target_link_libraries(dls_service
	PRIVATE
		dls_com
		dls_app
		dls_command
		dls_topics
		dls_domains
		dls_logging
)

dls_install(dls_service)
