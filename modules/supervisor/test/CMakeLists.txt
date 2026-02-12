add_executable(orchestrator_test
	supervisor.cpp
)

add_executable(cs_test
	fake_cs.cpp
)

set_property(TARGET orchestrator_test PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET cs_test PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(orchestrator_test
	PUBLIC
		include
)

target_include_directories(cs_test
	PUBLIC
		include
)

target_link_libraries(orchestrator_test
	PUBLIC
		dls_supervisor
)

target_link_libraries(cs_test
	PUBLIC
		dls_supervisor
)