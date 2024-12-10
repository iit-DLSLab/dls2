cmake_policy(SET CMP0048 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_compile_options("-std=c++2a")

set (LIBRARY_NAME dls_messages)
include(dls_message)          # message generation functions

add_library(${LIBRARY_NAME} SHARED)

# ================================== Messages =================================
include(idls_messages)

dls_install(${LIBRARY_NAME})