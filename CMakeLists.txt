cmake_minimum_required(VERSION 3.15)
project(dls2)
enable_testing()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# ==================== Project Configurations and Functions ====================
include(dls_setup)         # set githooks, project version
include(dls_configure)     # some utility functions
include(dls_globals)       # global project settings and definitions
include(dls_options)       # compile-time options
include(dls_deploy)        # installation functions
set(DLS_MESSAGE_HEADER_DESTINATION dls2/msg)
include(dls_message)       # message generation functions
include(dls_include_paths) # directories of global libraries

# ================================= Compilation ================================
add_subdirectory(msg)       # generate dds message types used in the project
add_subdirectory(scripts)   # bash completion and launch scripts
# add_subdirectory(src)     # compilation TODO move remaining parts into modules/
# add_subdirectory(test)    # TODO move tests to their respective modules
# add_subdirectory(doc)     # generate and install documentation
add_subdirectory(modules)   # build framework, libraries and tools
include(dls_catkin_ws)      # legacy code and code to interact with legacy code
add_subdirectory(3rd_party) # Some legacy and external code TODO move into better locations

include(dls_debian_settings) # configure installation of the framework

# TODO move this into doc/ subdirectory when things have been cleaned up over
# there
add_custom_target(graph
	COMMAND           ${CMAKE_COMMAND} "--graphviz=dependency_graph.dot" ${CMAKE_SOURCE_DIR}
	COMMAND           dot -Tpng dependency_graph.dot -o dependency_graph.png
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
