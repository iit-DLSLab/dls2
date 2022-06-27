message(STATUS "## Reading dls_include_paths.cmake")

include_directories(
	SYSTEM
	${EIGEN_DIR}
	${CMAKE_BINARY_DIR}/iit_commons/include
	${CMAKE_BINARY_DIR}/doglib/include
	${CMAKE_BINARY_DIR}/dls_messages/include
)
