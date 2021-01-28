cmake_minimum_required(VERSION 3.15)
project(dls2)
enable_testing()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(dls_setup) # set githooks, project version

add_subdirectory(3rd_party) # Some legacy and external code TODO move into better locations

include(dls_configure)     # some utility functions
include(dls_globals)       # global project settings and definitions
include(dls_options)       # compile-time options
include(dls_deploy)        # installation functions
set(DLS_MESSAGE_HEADER_DESTINATION dls2/msg)
include(dls_message)       # message generation functions
include(dls_include_paths) # directories of global libraries

add_subdirectory(msg)     # generate dds message types used in the project
add_subdirectory(scripts) # bash completion and launch scripts
# add_subdirectory(src)     # compilation TODO move remaining parts into modules/
# add_subdirectory(test)    # TODO move tests to their respective modules
# add_subdirectory(doc)     # generate and install documentation
add_subdirectory(modules) # build framework, libraries and tools

include(dls_debian_settings) # configure installation of the framework

# TODO move this into doc/ subdirectory when things have been cleaned up over
# there
add_custom_target(graph
	COMMAND           ${CMAKE_COMMAND} "--graphviz=dependency_graph.dot" ${CMAKE_SOURCE_DIR}
	COMMAND           dot -Tpng dependency_graph.dot -o dependency_graph.png
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# ==============================================================================
# Catkin
# ==============================================================================
set(CATKIN_WORKSPACE ${PROJECT_SOURCE_DIR}/catkin_ws)
add_custom_target(dls2_to_roscontrol
	COMMAND           catkin_make install
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/catkin_ws
	COMMENT           "Building dls2_to_roscontrol subproject"
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
add_dependencies(dls_framework
	dls2_to_roscontrol
)
add_dependencies(
	dls2_to_roscontrol
	dls_messaging
)

