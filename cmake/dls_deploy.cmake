# ==============================================================================
# Target Installation
# ==============================================================================
# ------------------------------------------------------------------------------
# Componenets
# ------------------------------------------------------------------------------
function(register_component COMPONENT_NAME)
	string(TOUPPER ${COMPONENT_NAME} COMPONENT_NAME_UPPER)

	# Assert that the component name starts with "dls_"
	string(FIND ${COMPONENT_NAME} "dls_" DLS_SUBSTRING_INDEX)
	if(NOT DLS_SUBSTRING_INDEX EQUAL 0)
		message(FATAL_ERROR "COMPONENT_NAME '${COMPONENT_NAME}' must start with 'dls_'")
	endif()

	# Extract the name without the "dls_" prefix
	string(SUBSTRING ${COMPONENT_NAME} 4 -1 COMPONENT_NAME_BARE)


	# Register the debian name for this component -- requries CPack to not be
	# included yet
	set(CPACK_DEBIAN_${COMPONENT_NAME_UPPER}_FILE_NAME
		"dls2-${COMPONENT_NAME_BARE}-${PROJECT_VERSION}.deb"
		PARENT_SCOPE
	)
	set(CPACK_DEBIAN_${COMPONENT_NAME_UPPER}_PACKAGE_NAME
		"dls2-${COMPONENT_NAME_BARE}"
		PARENT_SCOPE
	)
endfunction()

register_component(dls_dev)
register_component(dls_docs)
register_component(dls_runtime)

# ------------------------------------------------------------------------------
# Functions
# ------------------------------------------------------------------------------
# Target installation macro
function(dls_install INSTALL_TARGET)
	install(TARGETS ${INSTALL_TARGET}
		LIBRARY
			DESTINATION lib/dls2
			COMPONENT dls_runtime
		ARCHIVE
			DESTINATION lib
			COMPONENT dls_runtime
		RUNTIME
			DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
			COMPONENT dls_runtime
	)
	install(
		DIRECTORY
			include/
		DESTINATION
			include
		COMPONENT
			dls_dev
		FILES_MATCHING
			PATTERN "*.h"
			PATTERN "*.hpp"
			PATTERN "*.tpp"
	)
	message("Installing target ${INSTALL_TARGET}")
endfunction()

# Target to install only for debug builds, not in release
function(dls_install_debug INSTALL_TARGET)
	install(TARGETS ${INSTALL_TARGET}
		CONFIGURATIONS Debug
		LIBRARY
			DESTINATION lib/dls2
			COMPONENT dls_runtime
		ARCHIVE
			DESTINATION lib
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
		# ${PROJECT_SOURCE_DIR}/gen/include/dls2/
	DESTINATION
		include/dls2
	COMPONENT
		dls_dev
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
		PATTERN "*.tpp"
)


