cmake_minimum_required(VERSION 3.5.1)
project(dls2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MAJOR 0)
set(MINOR 0)
set(PATCH 0)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(BUILD_PREFIX "release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(BUILD_PREFIX "debug")
else()
	message(STATUS "Build type supplied: ${CMAKE_BUILD_TYPE}")
	message(WARNING "Build type not supported. Setting to Debug")
	set(CMAKE_BUILD_TYPE "Debug")
	set(BUILD_PREFIX "debug")
endif()
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

# IF(NOT $ENV{EIGEN3})
# 	MESSAGE(WARNING "EIGEN3 environment variable not set. Assuming /usr/include/eigen3")
# 	set(EIGEN_DIR /usr/include/eigen3)
# else()
# 	set(EIGEN_DIR $ENV{EIGEN3})
# ENDIF()
set(EIGEN_DIR /usr/include/eigen3)

string(LENGTH "{CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/build_tools/include
)
include_directories(
	SYSTEM
	${EIGEN_DIR}
)
include_directories(
	SYSTEM
	${CMAKE_SOURCE_DIR}/gen/include
)

add_subdirectory(src)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	enable_testing()
	add_subdirectory(test)
	add_subdirectory(doc)
endif()

add_custom_target(
	todos
	COMMAND ${CMAKE_SOURCE_DIR}/build_tools/list_todos
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	include(InstallRequiredSystemLibraries)
	set(CPACK_GENERATOR "DEB")
	set(CPACK_PACKAGE_VENDOR "dlslab")
	set(CPACK_PACKAGE_CONTACT "dlslab")
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "dlslab")
	set(CPACK_PACKAGE_VERSION_MAJOR ${MAJOR})
	set(CPACK_PACKAGE_VERSION_MINOR ${MINOR})
	set(CPACK_PACKAGE_VERSION_PATCH ${PATCH})
	include(CPack)
endif()
