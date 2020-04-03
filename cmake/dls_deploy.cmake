# ==============================================================================
# Debian Packaging Settings
# ==============================================================================
include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "dlslab")
set(CPACK_PACKAGE_CONTACT "dlslab")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "dlslab")
set(CPACK_DEBIAN_PACKAGE_NAME "dls")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VESRION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VESRION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VESRION_PATCH})
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_CMAKE_GENERTOR Ninja) # make CPack not rebuild everything
include(CPack)

# ==============================================================================
# Target Installation
# ==============================================================================
# ------------------------------------------------------------------------------
# Componenets
# ------------------------------------------------------------------------------
cpack_add_component_group("dls_framework")
cpack_add_component_group("dls_docs")
cpack_add_component_group("dls_dev")

cpack_add_component("dls_runtime"
	DISPLAY_NAME "runtime"
	# GROUP "dls_framework"
	GROUP "dls_runtime"
	DESCRIPTIONS "Binaries and executable scripts"
)

cpack_add_component("dls_dev"
	DISPLAY_NAME "development files"
	# GROUP "dls_framework"
	GROUP "dls_dev"
	DESCRIPTIONS "Files for development of plugins"
)

cpack_add_component("dls_docs"
	DISPLAY_NAME "docs"
	# GROUP "dls_framework"
	GROUP "dls_docs"
	DESCRIPTIONS "Development documentation"
)

# ------------------------------------------------------------------------------
# Functions
# ------------------------------------------------------------------------------
# Target installation macro
function(dls_install INSTALL_TARGET)
	install(TARGETS ${INSTALL_TARGET}
		LIBRARY
			DESTINATION lib/dls2
			COMPONENT "dls_runtime"
		ARCHIVE
			DESTINATION lib
			COMPONENT "dls_runtime"
		RUNTIME
			DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
			COMPONENT "dls_runtime"
	)
endfunction()

# Target to install only for debug builds, not in release
function(dls_install_debug INSTALL_TARGET)
	install(TARGETS ${INSTALL_TARGET}
		CONFIGURATIONS Debug
		LIBRARY
			DESTINATION lib/dls2
			COMPONENT "dls_runtime"
		ARCHIVE
			DESTINATION lib
			COMPONENT "dls_runtime"
		RUNTIME
			DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
			COMPONENT "dls_runtime"
	)
endfunction()

# ------------------------------------------------------------------------------
# Header file installation
# ------------------------------------------------------------------------------
install(
	DIRECTORY
		${PROJECT_SOURCE_DIR}/include/dls2/
		# ${PROJECT_SOURCE_DIR}/gen/include/dls2/
	DESTINATION
		include/dls2
	COMPONENT
		"dls_dev"
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
		PATTERN "*.tpp"
)
