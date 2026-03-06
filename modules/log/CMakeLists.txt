add_library(dls_logging
	src/event_logger.cpp
	src/terminal_logger.cpp
	src/file_logger.cpp
	src/app_logger.cpp
)

set_property(TARGET dls_logging PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_logging
	PUBLIC
		include
		third-party/magic_enum/include
)

target_link_libraries(dls_logging
	PUBLIC
		dls_com
		dls_time
		dls_domains
		dls_topics
		dls_time
)

add_subdirectory(test)

dls_install(dls_logging)

