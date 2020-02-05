# ==============================================================================
# PATHS
# ==============================================================================
set(DLS_INSTALL_LAUNCH_RUNTIME_DIR /usr/bin
	CACHE string "Directory where the launch script is installed to"
)

set(DLS_INSTALL_RUNTIME_DIR /usr/bin/dls2
	CACHE string "Directory where framework executables are stored"
)

set(DLS_INSTALL_GAIT_GENERATOR_DIR /usr/lib/dls2
	CACHE string "Directory where shared object libraries for gait generators are stored"
)

set(DLS_INSTALL_CONTROLLER_DIR /usr/lib/dls2
	CACHE string "Directory where shared object libraries for controllers are stored"
)

set(DLS_INSTALL_ESTIMATOR_DIR /usr/lib/dls2
	CACHE string "Directory where shared object libraries for estimators are stored"
)

set(DLS_INSTALL_SCRIPTS_DIR /usr/bin/dls2/scripts
	CACHE string "Directory where executable scripts are stored"
)

set(DLS_INSTALL_DEVELOPMENT_DOCS_DIR /usr/local/share/doc/dls2
	CACHE string "Directory where software development documentation is stored"
)

set(DLS_INSTALL_DOCUMENTATION On
	CACHE bool "Whether to install documentation"
)
