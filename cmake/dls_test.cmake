add_custom_target(tests)

add_custom_target(check
	COMMAND make test
	DEPENDS tests
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Running tests..."
)

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
	set(TARGET_NAME "test_${TEST_NAME}")

	set(prefix      create_test         )
	set(noValues                        )
	set(singleValues                    )
	set(multiValues SOURCE LINK INCLUDE )

	cmake_parse_arguments(
		${prefix}
		${noValues}
		${singleValues}
		${multiValues}
		${ARGN}
	)

	add_executable(${TARGET_NAME})

	target_include_directories(${TARGET_NAME}
		PRIVATE
			${${prefix}_INCLUDE
	)

	target_link_libraries(${TARGET_NAME}
		PRIVATE
			catch2
			${${prefix}_LINK}
	)

	set_target_properties(${TARGET_NAME}
		PROPERTIES
			EXCLUDE_FROM_ALL ON
	)

	add_dependencies(tests ${TARGET_NAME})


endfunction()
