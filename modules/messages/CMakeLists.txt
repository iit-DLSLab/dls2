cmake_policy(SET CMP0048 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_compile_options("-std=c++2a")

# ================================== Wrapper =================================
add_library(dls_msg_wrapper 
	SHARED
		src/wrapper_base.cpp
)
target_include_directories(dls_msg_wrapper
    PUBLIC
        include
)

# ================================== Messages =================================
set (LIBRARY_NAME dls_messages)
include(dls_message)          # message generation functions

add_library(${LIBRARY_NAME} SHARED)
include(idls_messages)

target_link_libraries(${LIBRARY_NAME}
PUBLIC
    robotlib
    dls_msg_wrapper
)

target_include_directories(${LIBRARY_NAME} 
PUBLIC
    ${EIGEN_DIR}
)

dls_install(${LIBRARY_NAME})
dls_install(dls_msg_wrapper)

install(
    DIRECTORY
		${CMAKE_CURRENT_LIST_DIR}/cmake/
    DESTINATION
		/usr/include/${LIBRARY_NAME}/cmake
	COMPONENT
		dls_dev
)