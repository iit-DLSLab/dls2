add_executable(orchestrator_test
	main.cpp
)

set_property(TARGET orchestrator_test PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(orchestrator_test
	PUBLIC
		include
)

target_link_libraries(orchestrator_test
	PUBLIC
		dls_supervisor
)