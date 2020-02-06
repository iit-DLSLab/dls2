cmake_minimum_required(VERSION 3.5.1)
project(dls2 VERSION 0.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

include(dls_options)
include(dls_globals)

add_subdirectory(3rd_party)
add_subdirectory(src)
add_subdirectory(scripts)
add_subdirectory(test)
add_subdirectory(doc)
