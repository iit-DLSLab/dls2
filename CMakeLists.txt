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
include(dls_include_paths)    # directories of global libraries
include(dls_dependency_graph) # `make graph` target to generate dependency graph
include(dls_test)             # functions to register unit tests

# ================================= Compilation ================================
add_subdirectory(modules)   # build framework, libraries and tools
add_subdirectory(scripts)   # bash completion and launch scripts
add_subdirectory(test)      # Test libraries and frameworks. Actual unit tests are in their respective moduels
add_subdirectory(doc)       # generate and install documentation

install(SCRIPT "cmake/dls_post_install.cmake")

# ================================== Packaging =================================
include(dls_debian_settings) # configure installation of the framework
