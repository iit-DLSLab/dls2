cmake_minimum_required(VERSION 3.5.1)
project(dls2 VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

# 3rd party libs do not use variables defined by cmake include targets. Do not
# move this below the includes
add_subdirectory(3rd_party)

include(dls_options)
include(dls_configure)
include(dls_globals)
include(dls_deploy)

add_subdirectory(src)
add_subdirectory(scripts)
# add_subdirectory(test)
add_subdirectory(doc)
