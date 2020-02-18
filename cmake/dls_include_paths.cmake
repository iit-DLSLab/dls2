include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/build_tools/include
)

include_directories(
	SYSTEM
	${EIGEN_DIR}
	${PROJECT_SOURCE_DIR}/3rd_party/iit_commons/include
	${PROJECT_SOURCE_DIR}/3rd_party/doglib/include
	${PROJECT_SOURCE_DIR}/gen/include
)
