cmake_minimum_required(VERSION 3.5.1)
project(dls2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MAJOR 0)
set(MINOR 0)
set(PATCH 0)

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

enable_testing()
# add_subdirectory(3rd_party)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)

add_custom_target(
	todos
	COMMAND ${CMAKE_SOURCE_DIR}/build_tools/list_todos
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "dlslab")
set(CPACK_PACKAGE_CONTACT "dlslab")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "dlslab")
set(CPACK_PACKAGE_VERSION_MAJOR ${MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PATCH})
include(CPack)
