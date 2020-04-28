# Include the framework headers from the source tree
include(dls_include_paths)

# Include Catch2 for testing
include_directories(
	SYSTEM
	include
	dls_${PROJECT_NAME}_msg_includes
)

# Create a custom target that builds all tests
add_custom_target(tests)

add_custom_target(check
	COMMAND make test
	DEPENDS tests
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Running tests..."
)

# Function to register a test
#
# Usage: create_test(<name_of_test> SOURCE <list of source files> LINK <list of
# libraries to link agains>)
#
# Test are not built by default. They are built when invoking `make tests`. The
# tests can be run with CTest with `make test`.
#
# The test name is prefixed with `test`
function(create_test TEST_NAME)
	set(TARGET_NAME "test_${TEST_NAME}")

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

	add_executable(${TARGET_NAME}        ${${prefix}_SOURCE})
	set_target_properties(${TARGET_NAME} PROPERTIES EXCLUDE_FROM_ALL 1)
	target_link_libraries(${TARGET_NAME} ${${prefix}_LINK})
	add_dependencies("tests"             ${TARGET_NAME})

	add_test(
		NAME              ${TEST_NAME}
		COMMAND           ${TARGET_NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)
endfunction()

add_subdirectory(src)
