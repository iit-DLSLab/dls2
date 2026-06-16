file(GLOB_RECURSE dls_message_idls CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/idls/*.idl"
)

foreach(idl_file IN LISTS dls_message_idls)
	file(RELATIVE_PATH idl_rel "${CMAKE_CURRENT_SOURCE_DIR}/idls" "${idl_file}")
	dls_add_message("${idl_rel}" ${LIBRARY_NAME})
endforeach()

dls_finalize_messages()
