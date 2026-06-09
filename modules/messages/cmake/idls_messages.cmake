file(GLOB dls_message_idls CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/idls/*.idl"
	"${CMAKE_CURRENT_SOURCE_DIR}/idls/ros2_interface/*/msg/*.idl"
)

foreach(idl_file IN LISTS dls_message_idls)
	file(RELATIVE_PATH idl_rel "${CMAKE_CURRENT_SOURCE_DIR}/idls" "${idl_file}")
	dls_add_message("${idl_rel}" ${LIBRARY_NAME})
endforeach()
