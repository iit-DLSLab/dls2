include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/build_tools/include
)

include_directories(
	SYSTEM
	${EIGEN_DIR}
	${CMAKE_SOURCE_DIR}/3rd_party/iit_commons/include
	${CMAKE_SOURCE_DIR}/3rd_party/doglib/include
	${CMAKE_SOURCE_DIR}/gen/include
)
