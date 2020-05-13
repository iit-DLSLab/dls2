# ==============================================================================
# PATHS
# ==============================================================================
set(DLS_INSTALL_LAUNCH_RUNTIME_DIR /usr/bin
	CACHE STRING "Directory where the launch script is installed to"
)

set(DLS_INSTALL_RUNTIME_DIR /usr/bin/dls2
	CACHE STRING "Directory where framework executables are stored"
)

set(DLS_INSTALL_GAIT_GENERATOR_DIR /usr/lib/dls2/gait_generators
	CACHE STRING "Directory where shared object libraries for gait generators are stored"
)

set(DLS_INSTALL_CONTROLLER_DIR /usr/lib/dls2/controllers
	CACHE STRING "Directory where shared object libraries for controllers are stored"
)

set(DLS_INSTALL_ESTIMATOR_DIR /usr/lib/dls2/estimators
	CACHE STRING "Directory where shared object libraries for estimators are stored"
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

set(DLS_CATKIN_WORKSPACE_DIR /opt/dls2/dls2_ws
	CACHE STRING "Directory where catkin workspace is installed to"
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
