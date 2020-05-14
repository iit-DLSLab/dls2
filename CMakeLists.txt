cmake_minimum_required(VERSION 3.5.1)
project(dls2)
enable_testing()
# ==============================================================================
# Setup the repository
# ==============================================================================
# This will perfom task such as setting up the git hooks for anyone who uses
# this project
execute_process(
	COMMAND           ./setup.sh
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# ==============================================================================
# Calculate correct project version number
# ==============================================================================
# Read in the current version
file(READ version.txt PROJECT_VERSION)

# create array of version strings
string(REPLACE "." ";" VERSION_ARRAY ${PROJECT_VERSION})
# ------------------------------------------------------------------------------
# Major, Minor, Patch numbers
# ------------------------------------------------------------------------------
# set corresponding cmake version variables
list(POP_FRONT VERSION_ARRAY
	PROJECT_VERSION_MAJOR
	PROJECT_VERSION_MINOR
	PROJECT_VERSION_PATCH
)
# Remove the trailing newline from the patch number
string(REPLACE "\n" "" PROJECT_VERSION_PATCH ${PROJECT_VERSION_PATCH})
# ------------------------------------------------------------------------------
# Tweak number
# ------------------------------------------------------------------------------
# If the current commit has a special tag, put that as the tweak number. Else,
# use the commit hash as the tweak number
execute_process(
	COMMAND         git tag --points-at HEAD
	OUTPUT_VARIABLE PROJECT_VERSION_TWEAK
)
# The command returns a newline-seprated list of tags at the current commit. It
# ends in a new line
#
# Replace all the newlines but the last with a dash
string(REGEX REPLACE
	"\n([^$])"                 # match string
	"-\\1"                     # replace string
	PROJECT_VERSION_TWEAK      # output var
	"${PROJECT_VERSION_TWEAK}" # input string
)
if(PROJECT_VERSION_TWEAK STREQUAL "")
	# This command leavs a trailing newline
	execute_process(
		COMMAND         git rev-parse HEAD
		OUTPUT_VARIABLE PROJECT_VERSION_TWEAK
	)
endif()
# Remove the trailing newline from the tweak number
string(REPLACE "\n" "" PROJECT_VERSION_TWEAK ${PROJECT_VERSION_TWEAK})

# ------------------------------------------------------------------------------
# Full version number
# ------------------------------------------------------------------------------
# Rebuild the project version including the current commit hash
string(JOIN "." PROJECT_VERSION
	${PROJECT_VERSION_MAJOR}
	${PROJECT_VERSION_MINOR}
	${PROJECT_VERSION_PATCH}
	${PROJECT_VERSION_TWEAK}
)
unset(VERSION_ARRAY) # delete aray of version strings
message(STATUS "PROJECT_VERSION ${PROJECT_VERSION}")
message(STATUS "PROJECT_VERSION_MAJOR ${PROJECT_VERSION_MAJOR}")
message(STATUS "PROJECT_VERSION_MINOR ${PROJECT_VERSION_MINOR}")
message(STATUS "PROJECT_VERSION_PATCH ${PROJECT_VERSION_PATCH}")
message(STATUS "PROJECT_VERSION_TWEAK ${PROJECT_VERSION_TWEAK}")

# ==============================================================================
# Project-Wide Settings
# ==============================================================================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

# 3rd party libs do not use variables defined by cmake include targets. Do not
# move this below the includes
include_directories(SYSTEM /opt/ros/kinetic/include)
add_subdirectory(3rd_party)

include(dls_configure)
include(dls_options)
include(dls_globals)
include(dls_deploy)

set(DLS_MESSAGE_HEADER_DESTINATION dls2/msg)
include(dls_message)       # depends on dls_deploy, do not include before it

add_compile_options(
	# Standards
	-std=c++14

	# Warnings
	-Wall
	-Wextra
	-pedantic
	# -Weffc++
	-g

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
add_subdirectory(test)
add_subdirectory(doc)
# add_subdirectory(catkin_ws)

# ==============================================================================
# Debian Packaging Settings
# ==============================================================================
#include(InstallRequiredSystemLibraries)
# General package settings
set(CPACK_PACKAGE_NAME          "dls2")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VESRION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VESRION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VESRION_PATCH})
set(CPACK_PACKAGE_VENDOR        "dlslab")
set(CPACK_PACKAGE_CONTACT       "dlslab")
set(CPACK_VERBATIM_VARIABLES    ON)

# Debian settings
set(CPACK_GENERATOR                       "DEB")
set(CPACK_DEB_COMPONENT_INSTALL           ON)
set(CPACK_DEBIAN_DOGLIB_FILE_NAME         "dls2-doglib-${PROJECT_VERSION}.deb")
set(CPACK_DEBIAN_PACKAGE_VERSION          ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER       "dlslab")
set(CPACK_DEBIAN_PACKAGE_NAME             "dls2")
set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
include(CPack)
