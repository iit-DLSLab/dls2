cmake_minimum_required(VERSION 3.5.1)
project(dls2 VERSION 0.0.0)
enable_testing()

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
# The dls_ws is packaged manually, since it is a catkin_ws, configure it here.
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/catkin_ws/packaging/dls2_ws/DEBIAN/control.in
	${CMAKE_CURRENT_SOURCE_DIR}/catkin_ws/packaging/dls2_ws/DEBIAN/control
	@ONLY
)

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
set(CPACK_DEBIAN_PACKAGE_MAINTAINER       "dlslab")
set(CPACK_DEBIAN_PACKAGE_NAME             "dls2")
set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
include(CPack)
