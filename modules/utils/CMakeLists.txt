configure_file(src/utils.cpp.in
	${CMAKE_CURRENT_BINARY_DIR}/src/utils.cpp
)

add_library(dls_utils
	${CMAKE_CURRENT_BINARY_DIR}/src/utils.cpp
)

target_link_libraries(dls_utils
	PUBLIC
		robotlib
		yaml-cpp
)

set_property(TARGET dls_utils PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_utils
	PUBLIC
		include
)

dls_install(dls_utils)