# ==============================================================================
# Man Pages
# ==============================================================================
# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/dls.1.in
	${CMAKE_BINARY_DIR}/doc/dls.1
	@ONLY
)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/dls.2.in
	${CMAKE_BINARY_DIR}/doc/dls.2
	@ONLY
)

# ------------------------------------------------------------------------------
# Installation
# ------------------------------------------------------------------------------
# command to run before installation
add_custom_target(man
	ALL
	DEPENDS
		${CMAKE_BINARY_DIR}/doc/dls.1.gz
		${CMAKE_BINARY_DIR}/doc/dls.2.gz
)

add_custom_command(
	OUTPUT
		${CMAKE_BINARY_DIR}/doc/dls.1.gz
		${CMAKE_BINARY_DIR}/doc/dls.2.gz
	COMMAND gzip ${CMAKE_BINARY_DIR}/doc/dls.1
	COMMAND gzip ${CMAKE_BINARY_DIR}/doc/dls.2
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
)

# actual installation
install(FILES ${CMAKE_BINARY_DIR}/doc/dls.1.gz
	DESTINATION /usr/man/man1
)

install(FILES ${CMAKE_BINARY_DIR}/doc/dls.2.gz
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
