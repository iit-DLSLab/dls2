# ==============================================================================
# Framework Launching
# ==============================================================================
dls_configure(launch_framework.in LAUNCH_FRAMEWORK)

install(FILES ${LAUNCH_FRAMEWORK}
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
		DESTINATION ${DLS_INSTALL_LAUNCH_RUNTIME_DIR}
	RENAME dls
)

# ==============================================================================
# Shell Completion
# ==============================================================================
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dls-completion.bash
	DESTINATION /etc/bash_completion.d
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dls-completion.zsh
	DESTINATION /usr/share/zsh/vendor-completions
	RENAME _dls
)

# ==============================================================================
# xdg-open
# ==============================================================================
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/xdg-open
	DESTINATION ${DLS_INSTALL_SCRIPTS_DIR}
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
)

# ==============================================================================
# CMake Macros for 3rd party plugins
# ==============================================================================
dls_configure_atonly(dls_install.cmake.in DLS_INSTALL_CMAKE)

install(FILES ${DLS_INSTALL_CMAKE}
	DESTINATION lib/cmake/dls
	RENAME dlsConfig.cmake
)
