add_library(param_server_msgs SHARED)
target_compile_options(param_server_msgs
	PRIVATE
	-w
)

function(param_server_gen_message msg)
	set(DDS_DIR ${CMAKE_CURRENT_BINARY_DIR}/include/dls2/msg)

	set(MSG_SOURCES
		${DDS_DIR}/${msg}.h
		${DDS_DIR}/${msg}.cxx
		${DDS_DIR}/${msg}PubSubTypes.h
		${DDS_DIR}/${msg}PubSubTypes.cxx
	)

	add_custom_command(
		OUTPUT
			${MSG_SOURCES}
		COMMAND
			[ -d ${DDS_DIR} ] || mkdir --parents ${DDS_DIR}
		COMMAND
			fastddsgen -replace ${CMAKE_CURRENT_SOURCE_DIR}/${msg}.idl -d ${DDS_DIR}
		COMMENT
			"Generating message files for ${msg}.idl"
		DEPENDS
			${msg}.idl
	)

	# add_custom_target(genfiles_${msg} DEPENDS

	target_sources(param_server_msgs
		PRIVATE
		${DDS_DIR}/${msg}.cxx
		${DDS_DIR}/${msg}PubSubTypes.cxx
	)
	target_include_directories(param_server_msgs
		PUBLIC
		${CMAKE_CURRENT_BINARY_DIR}/include
	)
endfunction()

param_server_gen_message(param_server_set_double)
