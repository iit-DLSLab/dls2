add_custom_command(
	OUTPUT "${CMAKE_SOURCE_DIR}/doc/html/index.html" "${CMAKE_SOURCE_DIR}/doc/latex/Makefile" "${CMAKE_SOURCE_DIR}/doc/man"
	COMMAND doxygen .Doxyfile
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
add_custom_command(
	OUTPUT "${CMAKE_SOURCE_DIR}/doc/latex/refman.pdf"
	DEPENDS "${CMAKE_SOURCE_DIR}/doc/latex/Makefile"
	COMMAND make
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/doc/latex"
)
add_custom_target(
	doc
	DEPENDS "${CMAKE_SOURCE_DIR}/doc/html/index.html" "${CMAKE_SOURCE_DIR}/doc/latex/refman.pdf" "${CMAKE_SOURCE_DIR}/doc/man" "${CMAKE_SOURCE_DIR}/.Doxyfile"
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
set_directory_properties(
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/html;${CMAKE_CURRENT_SOURCE_DIR}/latex;${CMAKE_CURRENT_SOURCE_DIR}/man"
)
