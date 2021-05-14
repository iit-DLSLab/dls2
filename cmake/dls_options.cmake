if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()                                                       
# ==============================================================================
# PATHS
# ==============================================================================
set(DLS_INSTALL_COMPONENT_BASE_DIR /usr/lib/dls2/components
	CACHE STRING "Base directory for layer components. Each layer adds a subdirectory to this path"
)

set(DLS_INSTALL_LAUNCH_RUNTIME_DIR /usr/bin
	CACHE STRING "Directory where the launch script is installed to"
)

set(DLS_INSTALL_RUNTIME_DIR /usr/bin/dls2
	CACHE STRING "Directory where framework executables are stored"
)

set(DLS_INSTALL_GAIT_GENERATOR_DIR /usr/lib/dls2/gait_generators
	CACHE STRING "Directory where shared object libraries for gait generators are stored"
)

set(DLS_INSTALL_GAIT_GENERATOR_HEADER_DIR /usr/include/dls2/gait_generators
	CACHE STRING "Directory where gait generator headers are stored"
)
set(DLS_INSTALL_CONTROLLER_DIR /usr/lib/dls2/controllers
	CACHE STRING "Directory where shared object libraries for controllers are stored"
)
set(DLS_INSTALL_CONTROLLER_HEADER_DIR /usr/include/dls2/controllers
	CACHE STRING "Directory where controller headers are stored"
)

set(DLS_INSTALL_ESTIMATOR_DIR /usr/lib/dls2/estimators
	CACHE STRING "Directory where shared object libraries for estimators are stored"
)
set(DLS_INSTALL_ESTIMATOR_HEADER_DIR /usr/include/dls2/estimators
	CACHE STRING "Directory where estimator headers are stored"
)

set(DLS_INSTALL_MESSAGES_DIR /usr/lib/dls2/messages
	CACHE STRING "Directory where shared object libraries for messages are stored"
)

set(DLS_INSTALL_SCRIPTS_DIR /usr/bin/dls2/scripts
	CACHE STRING "Directory where executable scripts are stored"
)

set(DLS_INSTALL_DEVELOPMENT_DOCS_DIR /usr/local/share/doc/dls2
	CACHE STRING "Directory where software development documentation is stored"
)

set(EIGEN_DIR /usr/include/eigen3
	CACHE STRING "Directory where Eigen is installed"
)



# ==============================================================================
# Documentation
# ==============================================================================
set(DLS_INSTALL_DEVELOPER_DOCUMENTATION On
	CACHE BOOL "Whether to install API documentation"
)

# ==============================================================================
# Convencience Targets
# ==============================================================================
set(DLS_BUILD_MAKE_RUN_FLAGS "--robot=hyq --core --layers=control,console"
	CACHE STRING "Flags to pass to framework when calling 'make run'"
)

#add_compile_options(-fPIC)
