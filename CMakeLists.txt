cmake_minimum_required(VERSION 3.15)
project(dls2)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
enable_testing()

# ==================== Project Configurations and Functions ====================
include(dls_setup)            # set githooks, project version
include(dls_configure)        # some utility functions
include(dls_globals)          # global project settings and definitions
include(dls_options)          # compile-time options
include(dls_deploy)           # installation functions
set(DLS_MESSAGE_HEADER_DESTINATION dls2/msg)
include(dls_message)          # message generation functions
include(dls_include_paths)    # directories of global libraries
include(dls_dependency_graph) # `make graph` target to generate dependency graph
include(dls_test)             # functions to register unit tests

# ================================= Compilation ================================
add_subdirectory(msg)       # generate dds message types used in the project
add_subdirectory(scripts)   # bash completion and launch scripts
add_subdirectory(test)      # Test libraries and frameworks. Actual unit tests are in their respective moduels
add_subdirectory(doc)     # generate and install documentation
add_subdirectory(modules)   # build framework, libraries and tools
add_subdirectory(3rd_party) # Some legacy and external code TODO move into better locations

# ================================= Packages configuration ================================
set(CPACK_DEBIAN_DLS_DEV_FILE_NAME "dls2-dev-${PROJECT_VERSION}.deb" PARENT_SCOPE)
set(CPACK_DEBIAN_DLS_DEV_PACKAGE_NAME "dls2-dev" PARENT_SCOPE)

set(CPACK_DEBIAN_DLS_DOCS_FILE_NAME "dls2-docs-${PROJECT_VERSION}.deb" PARENT_SCOPE)
set(CPACK_DEBIAN_DLS_DOCS_PACKAGE_NAME "dls2-docs" PARENT_SCOPE)

set(CPACK_DEBIAN_DLS_RUNTIME_FILE_NAME "dls2-runtime-${PROJECT_VERSION}.deb" PARENT_SCOPE)
set(CPACK_DEBIAN_DLS_RUNTIME_PACKAGE_NAME "dls2-runtime" PARENT_SCOPE)

set(CPACK_DEBIAN_DLS_MESSAGING_FILE_NAME "dls2-messaging-${PROJECT_VERSION}.deb" PARENT_SCOPE)
set(CPACK_DEBIAN_DLS_MESSAGING_PACKAGE_NAME	"dls2-messaging" PARENT_SCOPE)

# ================================== Packaging =================================
include(dls_debian_settings) # configure installation of the framework