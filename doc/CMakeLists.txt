message(STATUS "## Configuring doc folder")

include(dls_doc)
# ==============================================================================
# URL of example repository
# ==============================================================================
execute_process(
	COMMAND
		git config --get remote.origin.url
	WORKING_DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}/dls2_examples
	OUTPUT_VARIABLE
		DLS_EXAMPLES_REPOSITORY
)

# ==============================================================================
# Read Include Files
# ==============================================================================
# ------------------------------------------------------------------------------
# Flow chart
# ------------------------------------------------------------------------------
file(READ include/flow_chart.incl FLOW_CHART_INCL)

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

dls_configure_atonly(include/controller.md.in CONTROLLER_MD_FILE)
file(READ ${CONTROLLER_MD_FILE} CONTROLLER_MD)

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

# Generator CMake
file(READ dls2_examples/example_gait_generator/src/CMakeLists.txt
	EXAMPLE_GAIT_GENERATOR_CMAKE
)

dls_configure_atonly(include/gait_generator.md.in GAIT_GENERATOR_MD_FILE)
file(READ ${GAIT_GENERATOR_MD_FILE} GAIT_GENERATOR_MD)

# ------------------------------------------------------------------------------
# Logging
# ------------------------------------------------------------------------------
file(READ dls2_examples/example_log/src/main.cpp
	EXAMPLE_LOG_MAIN_CPP
)

dls_configure_atonly(include/logging.md.in LOGGING_MD_FILE)
file(READ ${LOGGING_MD_FILE} LOGGING_MD)

# ------------------------------------------------------------------------------
# Commands
# ------------------------------------------------------------------------------
file(READ dls2_examples/example_command/src/main.cpp
	EXAMPLE_COMMAND_MAIN_CPP
)

dls_configure_atonly(include/command.md.in COMMAND_MD_FILE)
file(READ ${COMMAND_MD_FILE} COMMAND_MD)

# ------------------------------------------------------------------------------
# PubSub
# ------------------------------------------------------------------------------
file(READ dls2_examples/example_pubsub/src/example_publisher.cpp
	EXAMPLE_PUBLISHER_CPP
)

file(READ dls2_examples/example_pubsub/src/example_subscriber.cpp
	EXAMPLE_SUBSCRIBER_CPP
)

dls_configure_atonly(include/pub_sub.md.in PUB_SUB_MD_FILE)
file(READ ${PUB_SUB_MD_FILE} PUB_SUB_MD)

# ------------------------------------------------------------------------------
# Service
# ------------------------------------------------------------------------------
file(READ dls2_examples/example_service/src/example_service_server.cpp
	EXAMPLE_SERVICE_SERVER_CPP
)

file(READ dls2_examples/example_service/src/example_service_client.cpp
	EXAMPLE_SERVICE_CLIENT_CPP
)

dls_configure_atonly(include/service.md.in SERVICE_MD_FILE)
file(READ ${SERVICE_MD_FILE} SERVICE_MD)

# ------------------------------------------------------------------------------
# Message Generation
# ------------------------------------------------------------------------------
# Build the graphical representation of the project tree
execute_process(
	COMMAND
		tree --noreport
	WORKING_DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}/dls2_examples/example_message_generation
	OUTPUT_VARIABLE
		EXAMPLE_MESSAGE_GENERATION_PROJECT_TREE
)

file(READ dls2_examples/example_message_generation/CMakeLists.txt
	EXAMPLE_MESSAGE_GENERATION_TOP_LEVEL_CMAKELSTS_TXT
)

file(READ dls2_examples/example_message_generation/msg/bool.idl
	EXAMPLE_MESSAGE_GENERATiON_BOOL_IDL
)

file(READ dls2_examples/example_message_generation/msg/CMakeLists.txt
	EXAMPLE_MESSAGE_GENERATION_MSG_CMAKELISTS_TXT
)

file(READ
	dls2_examples/example_message_generation/src/example_message_generation.cpp
	EXAMPLE_MESSAGE_GENERATiON_SRC_EXAMPLE_MESSAGE_GENERATION_CPP
)

file(READ
	dls2_examples/example_message_generation/src/CMakeLists.txt
	EXAMPLE_MESSAGE_GENERATION_SRC_CMAKELISTS_TXT
)

dls_configure_atonly(include/message_generation.md.in MESSAGE_GENERATION_MD_FILE)
file(READ ${MESSAGE_GENERATION_MD_FILE} MESSAGE_GENERATION_MD)

# ------------------------------------------------------------------------------
# Packaging
# ------------------------------------------------------------------------------
file(READ include/packaging.md PACKAGING_MD)

# ==============================================================================
# Man Pages
# ==============================================================================
# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------
# logging.md.in
dls_configure_destination(include/logging.md.in LOGGING_MD_DEST)
dls_markdown_to_man(${LOGGING_MD_FILE} LOGGING_MD_MAN_FILE)
file(READ ${LOGGING_MD_MAN_FILE} LOGGING_MD_MAN)

# command.md.in
dls_configure_destination(include/command.md.in COMMAND_MD_DEST)
dls_markdown_to_man(${COMMAND_MD_FILE} COMMAND_MD_MAN_FILE)
file(READ ${COMMAND_MD_MAN_FILE} COMMAND_MD_MAN)

# controller.md.in
dls_markdown_to_man(${CONTROLLER_MD_FILE} CONTROLLER_MD_MAN_FILE)
file(READ ${CONTROLLER_MD_MAN_FILE} CONTROLLER_MD_MAN)

# gait_generator.md.in
dls_markdown_to_man(${GAIT_GENERATOR_MD_FILE} GAIT_GENERATOR_MD_MAN_FILE)
file(READ ${GAIT_GENERATOR_MD_MAN_FILE} GAIT_GENERATOR_MD_MAN)

# pub_sub.md.in
dls_markdown_to_man(${PUB_SUB_MD_FILE} PUB_SUB_MD_MAN_FILE)
file(READ ${PUB_SUB_MD_MAN_FILE} PUB_SUB_MD_MAN)

# service.md.in
dls_markdown_to_man(${SERVICE_MD_FILE} SERVICE_MD_MAN_FILE)
file(READ ${SERVICE_MD_MAN_FILE} SERVICE_MD_MAN)

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
		${LOGGING_MD_MAN_FILE}
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
	# usage documentation should be available to everyone
	COMPONENT dls_runtime
)

if(DLS_INSTALL_DEVELOPER_DOCUMENTATION)
	install(FILES ${DLS_2_DEST}/dls.2.gz
		DESTINATION /usr/man/man2
		# development documentation should only be made available to devs
		COMPONENT dls_doc
	)
endif()

# ==============================================================================
# DOXYGEN
# ==============================================================================
# ------------------------------------------------------------------------------
# Configure
# ------------------------------------------------------------------------------
dls_configure_destination(Doxyfile.in DOXYFILE_DEST)

# Configure the main page
dls_configure_atonly(main.pre.md.in MAIN_MD_NO_TOC)
dls_configure_destination(main.pre.md.in MAIN_MD_NO_TOC_DEST)
set(MAIN_MD ${MAIN_MD_NO_TOC_DEST}/main.md)

# Generate the table of contents. @MAIN_MD@ is read by .Doxyfile
# execute_process(
# 	COMMAND
# 		pandoc ${MAIN_MD_NO_TOC} --table-of-contents --standalone -o ${MAIN_MD}
# )
dls_configure_atonly(main.pre.md.in MAIN_MD)


# Now run the configure with that value set
dls_configure_atonly(Doxyfile.in DOXYFILE)

set(DOXYGEN_EXTRACT_PRIVATE
	$<$<CONFIG:Debug>:YES>
	$<$CONFIG:Release>NO>
)
# ------------------------------------------------------------------------------
# Target for installation
# ------------------------------------------------------------------------------
add_custom_target(dls2-doxygen
	ALL
	#DEPENDS
	#	${DOXYFILE_DEST}/html
	#	#${DOXYFILE_DEST}/latex/refman.pdf
	#	${DOXYFILE_DEST}/man
	#	${DOXYFILE}
	COMMAND
		doxygen ${DOXYFILE}
	WORKING_DIRECTORY
		${PROJECT_SOURCE_DIR}
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
		${PROJECT_SOURCE_DIR}
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
		COMPONENT dls_doc
	)
endif()
