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

set(DLS_INSTALL_LIB_DIR /usr/lib/dls2
	CACHE STRING "Directory where framework shared object libraries are stored"
)

set(DLS_DEFAULT_HEADER_DIR /usr/include/dls2
	CACHE STRING "Directory where framework headers are stored"
)

set(DLS_ACTIONS_HEADER_DIR /usr/include/dls2/actions
	CACHE STRING "Directory where action headers are stored"
)

set(DLS_PROCEDURES_HEADER_DIR /usr/include/dls2/procedures
	CACHE STRING "Directory where procedure headers are stored"
)

set(DLS_SCHEDULERS_CONFIG_DIR /usr/include/dls2/schedulers
	CACHE STRING "Directory where framework scheduler config are stored"
)

set(DLS_INSTALL_SERVICE_DIR /usr/lib/dls2/services
	CACHE STRING "Directory where shared object libraries for services are stored"
)

set(DLS_INSTALL_SERVICE_HEADER_DIR /usr/include/dls2/services
	CACHE STRING "Directory where services headers are stored"
)

set(DLS_INSTALL_DATA_VISUALIZER_DIR /usr/lib/dls2/data_visualizer
	CACHE STRING "Directory where shared object libraries for data visualizer are stored"
)

set(DLS_INSTALL_DATA_VISUALIZER_HEADER_DIR /usr/include/dls2/data_visualizer
	CACHE STRING "Directory where data visualizer headers are stored"
)

set(DLS_INSTALL_ACTION_DIR /usr/lib/dls2/actions
	CACHE STRING "Directory where shared object libraries for actions are stored"
)

set(DLS_INSTALL_PROCEDURE_DIR /usr/lib/dls2/procedures
	CACHE STRING "Directory where shared object libraries for procedures are stored"
)

set(DLS_INSTALL_HARDWARE_DIR /usr/lib/dls2/hardwares
	CACHE STRING "Directory where shared object libraries for hardwares are stored"
)

set(DLS_INSTALL_HARDWARE_HEADER_DIR /usr/include/dls2/hardwares
	CACHE STRING "Directory where hardware headers are stored"
)

set(DLS_INSTALL_MOTION_GENERATOR_DIR /usr/lib/dls2/motion_generators
	CACHE STRING "Directory where shared object libraries for motion generators are stored"
)

set(DLS_INSTALL_MOTION_GENERATOR_HEADER_DIR /usr/include/dls2/motion_generators
	CACHE STRING "Directory where motion generator headers are stored"
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

set(DLS_INSTALL_MODEL_DIR /usr/lib/dls2/robots
	CACHE STRING "Directory where the robots libs are stored"
)

set(DLS_INSTALL_MODEL_HEADER_DIR /usr/include/dls2/robots
	CACHE STRING "Directory where the robots headers are stored"
)

set(DLS_INSTALL_MESSAGES_DIR /usr/lib/dls2/messages
	CACHE STRING "Directory where shared object libraries for messages are stored"
)

set(DLS_INSTALL_SCRIPTS_DIR /usr/bin/dls2/scripts
	CACHE STRING "Directory where executable scripts are stored"
)

set(DLS_INSTALL_DEVELOPMENT_DOCS_DIR /usr/share/doc/dls2
	CACHE STRING "Directory where software development documentation is stored"
)

set(DLS_INSTALL_STARTUP_DIR /usr/include/dls2/startup
	CACHE STRING "Directory where startup files are stored"
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
