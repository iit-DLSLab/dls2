add_library(dls_logging
	src/log.cpp
)

set_property(TARGET dls_logging PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_logging
	PUBLIC
		include
)

target_link_libraries(dls_logging
	PUBLIC
		dls_com
		dls_domains
		dls_topics
)

dls_install(dls_logging)

