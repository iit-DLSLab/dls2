set(CATKIN_WORKSPACE ${PROJECT_SOURCE_DIR}/catkin_ws)

add_custom_target(dls2_communication_bridges
	COMMAND           catkin_make install
	WORKING_DIRECTORY ${CATKIN_WORKSPACE}
	COMMENT           "Building dls2_communication_bridges subproject"
)

install(
	DIRECTORY              ${CATKIN_WORKSPACE}/install/
	DESTINATION            ${DLS_CATKIN_WORKSPACE_DIR}
	COMPONENT              dls_ws
	USE_SOURCE_PERMISSIONS
)

set(CPACK_DEBIAN_DLS_WS_FILE_NAME
	"dls2-workspace-${PROJECT_VERSION}.deb"
	PARENT_SCOPE
)

set(CPACK_DEBIAN_DLS_WS_PACKAGE_NAME
	"dls2-workspace"
	PARENT_SCOPE
)

set_property(
	DIRECTORY
		${PROJECT_SOURCE_DIR}
	PROPERTY ADDITIONAL_CLEAN_FILES
		${CATKIN_WORKSPACE}/build
		${CATKIN_WORKSPACE}/devel
		${CATKIN_WORKSPACE}/install
)

add_dependencies(dls2_communication_bridges
	dls_framework
)
