# ==============================================================================
# Target Installation
# ==============================================================================
# Target installation macro
function(dls_install INSTALL_TARGET)
	install(TARGETS ${INSTALL_TARGET}
		LIBRARY DESTINATION lib/dls2
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
		# PUBLIC_HEADER DESTINATION dls/include
	)
endfunction()

# Target to install only for debug builds, not in release
function(dls_install_debug INSTALL_TARGET)
	install(TARGETS ${INSTALL_TARGET}
		CONFIGURATIONS Debug
		# LIBRARY DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
		# ARCHIVE DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
		# RUNTIME DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
		LIBRARY DESTINATION lib/dls2
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
		# PUBLIC_HEADER DESTINATION dls/include
	)
endfunction()

# Header file installation
install(
	DIRECTORY
		${CMAKE_SOURCE_DIR}/include/dls2/
		${CMAKE_SOURCE_DIR}/gen/include/dls2/
	DESTINATION
		include/dls2
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
		PATTERN "*.tpp"
)

# ==============================================================================
# Debian Packaging Settings
# ==============================================================================
include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "dlslab")
set(CPACK_PACKAGE_CONTACT "dlslab")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "dlslab")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VESRION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VESRION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VESRION_PATCH})
set(CPACK_CMAKE_GENERTOR Ninja) # make CPack not rebuild everything
include(CPack)

