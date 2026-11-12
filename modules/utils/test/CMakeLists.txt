add_executable(dls_utils_waypointer_test
	test_waypointer.cpp
)

target_link_libraries(dls_utils_waypointer_test
	PRIVATE
		dls_utils
)

target_include_directories(dls_utils_waypointer_test
	PRIVATE
		../include
)

add_test(
	NAME dls_utils_waypointer_test
	COMMAND dls_utils_waypointer_test
)
