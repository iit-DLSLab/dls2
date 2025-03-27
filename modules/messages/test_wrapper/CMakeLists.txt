include(CTest)
find_package(GTest REQUIRED)
include(GoogleTest)

enable_testing()

define_property(GLOBAL PROPERTY ALL_TEST_TARGETS
    BRIEF_DOCS "All GTest Targets"
    FULL_DOCS "All GTest Targets")

set_property(GLOBAL PROPERTY ALL_TEST_TARGETS "")

function(add_wrapper_test)
    include(CMakeParseArguments)
    cmake_parse_arguments(PARSE_ARGV 0 "TEST" "" "TARGET" "SOURCES;LIBRARIES")
    message(STATUS "Adding Test: ${TEST_TARGET}...")

	add_executable(
		${TEST_TARGET} 
		${TEST_SOURCES}
	)

	target_link_libraries(
		${TEST_TARGET}
		GTest::gtest_main
		robotlib
        dls_messages
	)

    target_include_directories(${TEST_TARGET}
        PUBLIC 
            ${EIGEN_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/../include
    )
    
    target_compile_definitions(${TEST_TARGET} PRIVATE ROBOT="aliengo")

    add_test(${TEST_TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin/Release/${TEST_TARGET})
    set_property(GLOBAL APPEND PROPERTY ALL_TEST_TARGETS "${TEST_TARGET}")
endfunction()

add_wrapper_test(TARGET test_wrapper SOURCES test_wrapper.cpp)

get_property(my_list_content GLOBAL PROPERTY ALL_TEST_TARGETS)
add_custom_target(msg_wrapper_check COMMAND ctest --verbose GTEST_COLOR=1 CTEST_OUTPUT_ON_FAILURE=1 DEPENDS "${my_list_content}" COMMENT "Running tests...")
