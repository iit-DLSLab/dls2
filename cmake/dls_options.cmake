# ==============================================================================
# PATHS
# ==============================================================================
set(DLS_INSTALL_LAUNCH_RUNTIME_DIR /usr/bin
	CACHE STRING "Directory where the launch script is installed to"
)

set(DLS_INSTALL_RUNTIME_DIR /usr/bin/dls2
	CACHE STRING "Directory where framework executables are stored"
)

set(DLS_INSTALL_GAIT_GENERATOR_DIR /usr/lib/dls2
	CACHE STRING "Directory where shared object libraries for gait generators are stored"
)

set(DLS_INSTALL_CONTROLLER_DIR /usr/lib/dls2
	CACHE STRING "Directory where shared object libraries for controllers are stored"
)

set(DLS_INSTALL_ESTIMATOR_DIR /usr/lib/dls2
	CACHE STRING "Directory where shared object libraries for estimators are stored"
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
set(DLS_INSTALL_DOCUMENTATION On
	CACHE BOOL "Whether to install documentation"
)

# ==============================================================================
# Convencience Targets
# ==============================================================================
set(DLS_BUILD_MAKE_RUN_FLAGS "--robot-hyq --core"
	CACHE STRING "Flags to pass to framework when calling 'make run'"
)
