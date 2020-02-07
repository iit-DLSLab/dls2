# ==============================================================================
# Man Pages
# ==============================================================================
# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------
dls_configure_atonly(dls.1.in DLS_1)
dls_configure_atonly(dls.2.in DLS_2)
dls_configure_destination(dls.1.in DLS_1_DEST)
dls_configure_destination(dls.2.in DLS_2_DEST)

# ------------------------------------------------------------------------------
# Installation
# ------------------------------------------------------------------------------
# command to run before installation
add_custom_target(man
	ALL
	DEPENDS
		${DLS_1_DEST}/dls.1.gz
		${DLS_2_DEST}/dls.2.gz
)

add_custom_command(
	OUTPUT
		${DLS_1_DEST}/dls.1.gz
		${DLS_2_DEST}/dls.2.gz
	COMMAND gzip ${DLS_1}
	COMMAND gzip ${DLS_2}
	WORKING_DIRECTORY ${DLS_1_DEST}
)

# actual installation
install(FILES ${DLS_1_DEST}/dls.1.gz
	DESTINATION /usr/man/man1
)

install(FILES ${DLS_2_DEST}/dls.2.gz
	DESTINATION /usr/man/man2
)
# ==============================================================================
# ==============================================================================
# ==============================================================================
# DOXYGEN
# ==============================================================================
# ==============================================================================
# ==============================================================================
# ==============================================================================
# Make Directories
# ==============================================================================
add_custom_command(
	OUTPUT
		"${CMAKE_SOURCE_DIR}/doc/latex/"
	COMMAND
		[[ -d ${CMAKE_SOURCE_DIR}/doc/latex ]] || mkdir ${CMAKE_SOURCE_DIR}/doc/latex
)
add_custom_command(
	OUTPUT
		"${CMAKE_SOURCE_DIR}/doc/man/"
	COMMAND
		[[ -d ${CMAKE_SOURCE_DIR}/doc/man ]] || mkdir ${CMAKE_SOURCE_DIR}/doc/man
)
add_custom_command(
	OUTPUT
		"${CMAKE_SOURCE_DIR}/doc/html/"
	COMMAND
		[[ -d ${CMAKE_SOURCE_DIR}/doc/html ]] || mkdir ${CMAKE_SOURCE_DIR}/doc/html
)


# ==============================================================================
# Create Documentation
# ==============================================================================
add_custom_command(
	OUTPUT
		"${CMAKE_SOURCE_DIR}/doc/html/index.html"
		"${CMAKE_SOURCE_DIR}/doc/latex/Makefile"
		"${CMAKE_SOURCE_DIR}/doc/man"
	DEPENDS
		"${CMAKE_SOURCE_DIR}/doc/html/"
		"${CMAKE_SOURCE_DIR}/doc/latex/"
		"${CMAKE_SOURCE_DIR}/doc/man/"
	COMMAND
		doxygen .Doxyfile
	WORKING_DIRECTORY
		"${CMAKE_SOURCE_DIR}"
)

# ==============================================================================
# Compile PDF
# ==============================================================================
add_custom_command(
	OUTPUT
		"${CMAKE_SOURCE_DIR}/doc/latex/refman.pdf"
	DEPENDS
		"${CMAKE_SOURCE_DIR}/doc/latex/Makefile"
	COMMAND
		make
	WORKING_DIRECTORY
		"${CMAKE_SOURCE_DIR}/doc/latex"
)

# ==============================================================================
# Create Doc Target
# ==============================================================================
add_custom_target(
	doc
	DEPENDS
		"${CMAKE_SOURCE_DIR}/doc/html/index.html"
		# Switch off pdf generation
		#"${CMAKE_SOURCE_DIR}/doc/latex/refman.pdf"
		"${CMAKE_SOURCE_DIR}/doc/man" "${CMAKE_SOURCE_DIR}/.Doxyfile"
	WORKING_DIRECTORY
		"${CMAKE_SOURCE_DIR}"
)

# ==============================================================================
# Clean Directories
# ==============================================================================
set_directory_properties(
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/html;${CMAKE_CURRENT_SOURCE_DIR}/latex;${CMAKE_CURRENT_SOURCE_DIR}/man"
)

if(DLS_INSTALL_DOCUMENTATION)

	install(
		DIRECTORY ${CMAKE_SOURCE_DIR}/doc/html
		DESTINATION ${DLS_INSTALL_DEVELOPMENT_DOCS_DIR}
	)
endif()
