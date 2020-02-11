# ==============================================================================
# Read Include Files
# ==============================================================================
# ------------------------------------------------------------------------------
# Flow chart
# ------------------------------------------------------------------------------
file(READ flow_chart.incl FLOW_CHART_INCL)

# ------------------------------------------------------------------------------
# Logging
# ------------------------------------------------------------------------------
file(READ log/log_example.cpp.incl LOG_EXAMPLE_CPP_INCL)
file(READ log/log_types.incl LOG_TYPES_INCL)

# ------------------------------------------------------------------------------
# Example Controller
# ------------------------------------------------------------------------------
# Controller Header
file(READ dls2_examples/example_controller/include/example_controller.hpp
	EXAMPLE_CONTROLLER_HPP
)

# Controller Source
file(READ dls2_examples/example_controller/src/example_controller.cpp
	EXAMPLE_CONTROLLER_CPP
)

# Controller CMake
file(READ dls2_examples/example_controller/src/CMakeLists.txt
	EXAMPLE_CONTROLLER_CMAKE
)

# ------------------------------------------------------------------------------
# Example Generator
# ------------------------------------------------------------------------------
# Generator Header
file(READ dls2_examples/example_gait_generator/include/example_gait_generator.hpp
	EXAMPLE_GAIT_GENERATOR_HPP
)

# Generator Source
file(READ dls2_examples/example_gait_generator/src/example_gait_generator.cpp
	EXAMPLE_GAIT_GENERATOR_CPP
)
file(READ dls2_examples/example_gait_generator/src/CMakeLists.txt
	EXAMPLE_GAIT_GENERATOR_CMAKE
)

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
# Build manpages target
add_custom_target(man
	ALL
	DEPENDS
		${DLS_1_DEST}/dls.1.gz
		${DLS_2_DEST}/dls.2.gz
)

# Build manpages
add_custom_command(
	OUTPUT
		${DLS_1_DEST}/dls.1.gz
		${DLS_2_DEST}/dls.2.gz
	COMMAND gzip ${DLS_1}
	COMMAND gzip ${DLS_2}
	WORKING_DIRECTORY ${DLS_1_DEST}
)

# Install manpages
install(FILES ${DLS_1_DEST}/dls.1.gz
	DESTINATION /usr/man/man1
)

if(DLS_INSTALL_DEVELOPER_DOCUMENTATION)
	install(FILES ${DLS_2_DEST}/dls.2.gz
		DESTINATION /usr/man/man2
	)
endif()

# ==============================================================================
# DOXYGEN
# ==============================================================================
# ------------------------------------------------------------------------------
# Configure
# ------------------------------------------------------------------------------
# Doxyfile.in contains @DOXYFILE_DEST@ as ouput directory. Set its value here
dls_configure_destination(Doxyfile.in DOXYFILE_DEST)

# Configure the main page and set @MAIN_MD@ read by doxygen
dls_configure_atonly(main.md.in MAIN_MD)
# message(FATAL_ERROR "MAIN MD ${MAIN_MD}")

# Now run the configure with that value set
dls_configure_atonly(Doxyfile.in DOXYFILE)

set(DOXYGEN_EXTRACT_PRIVATE
	$<$<CONFIG:Debug>:YES>
	$<$CONFIG:Release>NO>
)
# ------------------------------------------------------------------------------
# Target for installation
# ------------------------------------------------------------------------------
add_custom_target(doxygen
	ALL
	#DEPENDS
	#	${DOXYFILE_DEST}/html
	#	#${DOXYFILE_DEST}/latex/refman.pdf
	#	${DOXYFILE_DEST}/man
	#	${DOXYFILE}
	COMMAND
		doxygen ${DOXYFILE}
	WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
)

# ------------------------------------------------------------------------------
# Create Documentation
# ------------------------------------------------------------------------------
add_custom_command(
	OUTPUT
		${DOXYFILE_DEST}/html
		${DOXYFILE_DEST}/latex
		${DOXYFILE_DEST}/man
	COMMAND
		doxygen ${DOXYFILE}
	WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
)

# ------------------------------------------------------------------------------
# Compile PDF
# ------------------------------------------------------------------------------
add_custom_command(
	OUTPUT
		${DOXYFILE_DEST}/latex/refman.pdf
	DEPENDS
		${DOXYFILE_DEST}/latex/Makefile
	COMMAND
		make
	WORKING_DIRECTORY
		${DOXYFILE_DEST}/latex
)

# ------------------------------------------------------------------------------
# Clean Directories
# ------------------------------------------------------------------------------
set_directory_properties(
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
	${DOXYFILE_DEST}/html
)

set_directory_properties(
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
	${DOXYFILE_DEST}/latex
)

set_directory_properties(
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
	${DOXYFILE_DEST}/man
)

# ------------------------------------------------------------------------------
# Install
# ------------------------------------------------------------------------------
if(DLS_INSTALL_DEVELOPER_DOCUMENTATION)
	install(
		DIRECTORY ${DOXYFILE_DEST}/html
		DESTINATION ${DLS_INSTALL_DEVELOPMENT_DOCS_DIR}
	)
endif()
