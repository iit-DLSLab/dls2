cmake_minimum_required(VERSION 3.5.1)
project(dls2 VERSION 0.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

# 3rd party libs do not use variables defined by cmake include targets. Do not
# move this below the includes
add_subdirectory(3rd_party)

include(dls_configure)
include(dls_options)
include(dls_globals)
include(dls_deploy)

include(dls_message) # depends on dls_deploy, do not include before it

add_compile_options(
	# Standards
	-std=c++14

	# Warnings
	-Wall
	-Wextra
	-pedantic
	-Weffc++

	# Elevate warnings to errors
	-Werror

	# TODO optimistation options

	# Custom defintions
	-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}
	-include ${PROJECT_SOURCE_DIR}/include/dls2/util/debug/debug.hpp
)


add_subdirectory(msg)
add_subdirectory(src)
add_subdirectory(scripts)
# add_subdirectory(test)
add_subdirectory(doc)
