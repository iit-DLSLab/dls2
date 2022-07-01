message(STATUS "## Reading dls_test.cmake")

add_custom_target(dls2-tests)

add_custom_target(check
	COMMAND make test
	DEPENDS dls2-tests
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Running tests..."
)

include(CMakeParseArguments)

# Function to register a test
#
# Usage: create_test(<name_of_test> SOURCE <list of source files> LINK <list of
# libraries to link agains> INCLUDE <list of include directories>)
#
# Tests are not built by default. They are built when invoking `make tests`. The
# tests can be run with CTest with `make test`.
#
# The test name is prefixed with `test`
function(dls_register_test TEST_NAME)
	set(prefix       create_test         )
	set(noValues     ""                  )
	set(singleValues ""                  )
	set(multiValues  SOURCE LINK INCLUDE )

	cmake_parse_arguments(
		${prefix}
		"${noValues}"
		"${singleValues}"
		"${multiValues}"
		${ARGN}
	)

	if(NOT DLS_TEST_SCOPE)
		message(FATAL_ERROR
			"Need to set variable DLS_TEST_SCOPE before calling dls_register_test"
		)
	endif()

	set(TARGET_NAME "test_${DLS_TEST_SCOPE}_${TEST_NAME}")

	add_executable(${TARGET_NAME}
		EXCLUDE_FROM_ALL
		${${prefix}_SOURCE}
	)

	target_include_directories(${TARGET_NAME}
		PRIVATE
			${${prefix}_INCLUDE}
	)

	target_link_libraries(${TARGET_NAME}
		PRIVATE
			catch2
			${${prefix}_LINK}
	)

	add_dependencies(dls2-tests ${TARGET_NAME})

	add_test(
		NAME    ${DLS_TEST_SCOPE}_${TEST_NAME}
		COMMAND ${TARGET_NAME}
	)

endfunction()
