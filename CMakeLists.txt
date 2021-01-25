cmake_minimum_required(VERSION 3.15)
project(dls2)
enable_testing()
# ==============================================================================
# Project-Wide Settings
# ==============================================================================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# run basic project setup
include(dls_setup)

add_subdirectory(3rd_party)

include(dls_configure)
include(dls_globals) # global project settings and definitions
include(dls_options) # compile-time options
include(dls_deploy)  # installation functions
set(DLS_MESSAGE_HEADER_DESTINATION dls2/msg)
include(dls_message) # message generation

add_subdirectory(msg)
add_subdirectory(scripts)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)

include(dls_debian_settings)
