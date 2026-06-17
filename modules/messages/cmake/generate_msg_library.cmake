# Configuration: define
set(generated_cpp_source "")
set(generated_cpp_headers "")
set(generated_py_source "")
set(MESSAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/dls_messages/dds")

# call fastsddsgen to generate source files for cpp and python
function(call_fastddsgen idl_file_path idl_file_name)
    # define files produced by fastddsgen
    set(generated_cpp_source
		"${MESSAGE_DIR}/${idl_file_name}PubSubTypes.cxx"
		"${MESSAGE_DIR}/${idl_file_name}TypeObjectSupport.cxx"
    )

    set(generated_cpp_headers
		"${MESSAGE_DIR}/${idl_file_name}.hpp"
		"${MESSAGE_DIR}/${idl_file_name}PubSubTypes.hpp"
		"${MESSAGE_DIR}/${idl_file_name}TypeObjectSupport.hpp"
		"${MESSAGE_DIR}/${idl_file_name}CdrAux.hpp"
		"${MESSAGE_DIR}/${idl_file_name}CdrAux.ipp"
    )	
    set(generated_py_source
		"${MESSAGE_DIR}/${idl_file_name}.i"
		"${MESSAGE_DIR}/${idl_file_name}PubSubTypes.i"
	)
# execute fastddsgen as custom command
# return command OUTPUTS
    set(fastddsgen_command
        fastddsgen
        -typeros2
        -replace
        -cs
        -python
        ${idl_file_path}
        -d
        ${MESSAGE_DIR}
	)
    add_custom_command(
		OUTPUT
			${generated_cpp_source}
			${generated_cpp_headers}
            ${generated_py_source}
		COMMAND
			[ -d ${MESSAGE_DIR} ] || mkdir --parents ${MESSAGE_DIR}
		COMMAND
            ${fastddsgen_command}
		COMMENT
			"Generating message files for ${idl_file_name}.idl"
		DEPENDS
		${idl_file_path}
	)
    
    add_library(${idl_file_name}_msg_cpp SHARED
        ${generated_cpp_source}
        )
    target_include_directories(${idl_file_name}_msg_cpp
        PUBLIC
            ${MESSAGE_DIR}
    )
    target_link_libraries(${idl_file_name}_msg_cpp
        PUBLIC
            fastcdr
            fastdds
    )
endfunction()

# add cpp msg library
function(add_cpp_msg_library library_name)
    add_library(${library_name} SHARED
        ${generated_cpp_source}
        )
    target_include_directories(${library_name}
        PUBLIC
            ${MESSAGE_DIR}
    )
    target_link_libraries(${library_name}
        PUBLIC
            fastcdr
            fastdds
    )
endfunction()
# add python msg library