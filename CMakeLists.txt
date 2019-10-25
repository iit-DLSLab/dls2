cmake_minimum_required(VERSION 3.5.1)
project(dls2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# IF(NOT $ENV{EIGEN3})
# 	MESSAGE(FATAL_ERROR "Set environment variable EIGEN3 to the include directory of your Eigen installation")
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

enable_testing()
# add_subdirectory(3rd_party)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)
