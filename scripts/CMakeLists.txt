# ==============================================================================
# Framework Launching
# ==============================================================================
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/launch_framework.in
	${CMAKE_BINARY_DIR}/scripts/launch_framework
)

install(
	FILES ${CMAKE_BINARY_DIR}/scripts/launch_framework
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
	DESTINATION /usr/bin
	RENAME dls
)

# ==============================================================================
# Shell Completion
# ==============================================================================
install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/dls-completion.bash
	DESTINATION /etc/bash_completion.d
)

install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/dls-completion.zsh
	DESTINATION /usr/share/zsh/vendor-completions
	RENAME _dls
)

# ==============================================================================
# xdg-open
# ==============================================================================
install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/xdg-open
	DESTINATION ${DLS_INSTALL_SCRIPTS_DIR}
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
)

# ==============================================================================
# CMake Macros for 3rd party plugins
# ==============================================================================
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/dls_install.cmake.in
	${CMAKE_BINARY_DIR}/scripts/dls_install.cmake
	@ONLY
)

install(
	FILES ${CMAKE_BINARY_DIR}/scripts/dls_install.cmake
	DESTINATION lib/cmake/dls
	RENAME dlsConfig.cmake
)
