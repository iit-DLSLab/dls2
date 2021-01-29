include_directories(
	SYSTEM
	${EIGEN_DIR}
	${PROJECT_SOURCE_DIR}/3rd_party/iit_commons/include
	${PROJECT_SOURCE_DIR}/3rd_party/doglib/include
	${PROJECT_SOURCE_DIR}/gen/include
	${dls_${PROJECT_NAME}_msg_includes}
)
