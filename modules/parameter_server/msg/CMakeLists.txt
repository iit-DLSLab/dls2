add_library(param_server_msgs SHARED)

function(param_server_gen_message msg)
	set(DDS_DIR ${CMAKE_CURRENT_BINARY_DIR}/include/dls2)

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
endfunction()

param_server_gen_message(param_server_set_double)
