add_executable(frenet_path_test
	frenet_path_test.cpp
)

set_property(TARGET frenet_path_test PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(frenet_path_test
	PUBLIC
		include
)

target_link_libraries(frenet_path_test
	PUBLIC
		dls_math
)