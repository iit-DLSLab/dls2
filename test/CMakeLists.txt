# Target to invoke to build tests
add_custom_target(tests)
add_compile_options(
	# Standards
	-std=c++17
)

function(create_test NAME SRC LINK)
	# Output directory of executable should mirror their location in source tree
	string(REGEX MATCH "/[^/]*$" PATH ${CMAKE_CURRENT_SOURCE_DIR})
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${BUILD_PREFIX}/test${PATH}/ PARENT_SCOPE)
	message("building test ${NAME} at ${EXECUTABLE_OUTPUT_PATH}")

	# create the executable
	add_executable("${NAME}" "${SRC}")
	target_link_libraries("${NAME}" "${LINK}")

	# do not build the test by default
	set_target_properties("${NAME}" PROPERTIES EXCLUDE_FROM_ALL 1)

	# but build it if we want to build all the tests
	add_dependencies("tests" "${NAME}")

	# register this test with CTest
	add_test(
		NAME ${NAME}
		COMMAND ${NAME}
		WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
	)
endfunction()

# add_subdirectory(geometry)
# add_subdirectory(msg_wrappers)
# add_subdirectory(util)
add_subdirectory(command)
