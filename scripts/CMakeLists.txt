# ==============================================================================
# Framework Launching
# ==============================================================================
# Collect all the paths where different parts of the Framework are installed,
# remove any duplicates and add them to the dynamic link path of the framework
list(APPEND DLS_LD_LIBRARY_PATH
	${DLS_INSTALL_GAIT_GENERATOR_DIR}
	${DLS_INSTALL_CONTROLLER_DIR}
	${DLS_INSTALL_ESTIMATOR_DIR}
	${DLS_INSTALL_MESSAGES_DIR}
)
list(REMOVE_DUPLICATES DLS_LD_LIBRARY_PATH)
list(JOIN DLS_LD_LIBRARY_PATH ":" DLS_LD_LIBRARY_PATH)

# Configure the version of the launch script that is used to run the installed
# framework
set(DLS_CHILD_PROCESS_LAUNCHER              "${DLS_INSTALL_RUNTIME_DIR}/child_process_launcher")
set(DLS_LAUNCH_SCRIPT_LIBRARY_PATH          "/usr/lib/dls2:${DLS_LD_LIBRARY_PATH}:$LD_LIBRARY_PATH")
set(DLS_LUANCH_SCRIPT_RUN_FRAMEWORK_COMMAND "${DLS_INSTALL_RUNTIME_DIR}/dynamic_legged_systems_framework \"$@\"")
dls_configure(launch_framework.in           LAUNCH_FRAMEWORK)

# Configure the version of the launch script that is used to launch the framework
# from the build tree during active development
set(DLS_CHILD_PROCESS_LAUNCHER "${EXECUTABLE_OUTPUT_PATH}/child_process_launcher")
set(DLS_CATKIN_WORKSPACE_SETUP_BASH ${PROJECT_SOURCE_DIR}/catkin_ws/devel/setup.bash)
set(DLS_LAUNCH_SCRIPT_LIBRARY_PATH "$LD_LIBRARY_PATH")
set(DLS_LUANCH_SCRIPT_RUN_FRAMEWORK_COMMAND "${EXECUTABLE_OUTPUT_PATH}/dynamic_legged_systems_framework \"$@\"")
configure_file(launch_framework.in "${CMAKE_CURRENT_BINARY_DIR}/launch_framework_from_src_tree")
# Create an interface target for this launch script so that it can be found from other parts of the build system
add_library(launch_framework_from_src_tree INTERFACE)
set_property(TARGET launch_framework_from_src_tree
	PROPERTY INTERFACE_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/launch_framework_from_src_tree"
)

install(FILES ${LAUNCH_FRAMEWORK}
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
		DESTINATION ${DLS_INSTALL_LAUNCH_RUNTIME_DIR}
	RENAME dls
	COMPONENT
		dls_runtime
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dls2.conf
	DESTINATION /etc/ld.so.conf.d
	COMPONENT
		dls_runtime
)

# ==============================================================================
# Shell Completion
# ==============================================================================
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dls-completion.bash
	DESTINATION /etc/bash_completion.d
	COMPONENT
		dls_runtime
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dls-completion.zsh
	DESTINATION /usr/share/zsh/vendor-completions
	RENAME _dls
	COMPONENT
		dls_runtime
)

# ==============================================================================
# xdg-open
# ==============================================================================
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/xdg-open
	DESTINATION ${DLS_INSTALL_SCRIPTS_DIR}
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
	COMPONENT
		dls_runtime
)

# ==============================================================================
# CMake Macros for 3rd party plugins
# ==============================================================================
dls_configure_atonly(dls_install.cmake.in DLS_INSTALL_CMAKE)

install(FILES ${DLS_INSTALL_CMAKE}
	DESTINATION lib/cmake/dls
	RENAME dlsConfig.cmake
	COMPONENT
		dls_dev
)

# ==============================================================================
# fix_msg
# ==============================================================================
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/fix_msg.bash
	DESTINATION ${DLS_INSTALL_SCRIPTS_DIR}
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
	COMPONENT
		dls_runtime
)
