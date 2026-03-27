configure_file(src/utils.cpp.in
	${CMAKE_CURRENT_BINARY_DIR}/src/utils.cpp
)

add_library(dls_utils
	${CMAKE_CURRENT_BINARY_DIR}/src/utils.cpp
	src/pose.cpp
	src/screw.cpp
	src/process_resource_monitor.cpp
	src/system_resource_monitor.cpp
)

target_link_libraries(dls_utils
	PUBLIC
		Eigen3::Eigen
		robotlib
		yaml-cpp
		dls_math
		dls_time
)

set_property(TARGET dls_utils PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_utils
	PUBLIC
		include
)

dls_install(dls_utils)