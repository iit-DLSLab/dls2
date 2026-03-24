find_package(nlohmann_json 3.11.3 REQUIRED)

add_executable(test_notify_events
	test_notify_events.cpp
)

set_property(TARGET test_notify_events PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(test_notify_events
	PUBLIC
		include
)

target_link_libraries(test_notify_events
	PUBLIC
		dls_com
		dls_domains
		dls_topics
		dls_logging
		nlohmann_json::nlohmann_json
)