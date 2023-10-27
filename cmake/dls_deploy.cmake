# ------------------------------------------------------------------------------
# Functions
# ------------------------------------------------------------------------------
# Target installation macro
function(dls_install INSTALL_TARGET)
	install(TARGETS ${INSTALL_TARGET}
		LIBRARY
			DESTINATION /usr/lib/dls2
			COMPONENT dls_runtime
		ARCHIVE
			DESTINATION /usr/lib
			COMPONENT dls_runtime
		RUNTIME
			DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
			COMPONENT dls_runtime
	)
	install(
		DIRECTORY
			include/
		DESTINATION
			/usr/include
		COMPONENT
			dls_dev
		FILES_MATCHING
			PATTERN "*.h"
			PATTERN "*.hpp"
			PATTERN "*.tpp"
			PATTERN "*.yaml"
	)
endfunction()

# Target to install only for debug builds, not in release
function(dls_install_debug INSTALL_TARGET)
	install(TARGETS ${INSTALL_TARGET}
		CONFIGURATIONS Debug
		LIBRARY
			DESTINATION /usr/lib/dls2
			COMPONENT dls_runtime
		ARCHIVE
			DESTINATION /usr/lib
			COMPONENT dls_runtime
		RUNTIME
			DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
			COMPONENT dls_runtime
	)
	message("Installing target ${INSTALL_TARGET}")
endfunction()

# ------------------------------------------------------------------------------
# Header file installation
# ------------------------------------------------------------------------------
install(
	DIRECTORY
		${PROJECT_SOURCE_DIR}/include/dls2/
	DESTINATION
		/usr/include/dls2
	COMPONENT
		dls_dev
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
		PATTERN "*.tpp"
)