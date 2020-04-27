include(dls_include_paths)

include_directories(
	SYSTEM
	include
)

add_custom_target(tests)
function(create_test TEST_NAME)

	set(prefix       create_test)
	set(noValues)     # unused
	set(singleValues) # unused
	set(multiValues  SOURCE LINK)

	cmake_parse_arguments(
		${prefix}
		"${noValues}"
		"${singleValues}"
		"${multiValues}"
		${ARGN}
	)

	add_executable(${TEST_NAME} ${${prefix}_SOURCE})
	set_target_properties(${TEST_NAME} PROPERTIES EXCLUDE_FROM_ALL 1)
	target_link_libraries(${TEST_NAME} ${${prefix}_LINK})
	add_dependencies("tests" ${TEST_NAME})
	add_test(
		NAME              ${TEST_NAME}
		COMMAND           ${TEST_NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)

endfunction()

add_subdirectory(src)
