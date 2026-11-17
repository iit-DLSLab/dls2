set(CPP_PYTHON_COMMUNICATION_WRITER cpp_python_communication_all_writers)
set(DLS_MESSAGES_IDL_ROOT "${PROJECT_SOURCE_DIR}/modules/messages/idls")
set(CPP_PYTHON_COMMUNICATION_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

file(GLOB_RECURSE cpp_python_communication_idls CONFIGURE_DEPENDS
    "${DLS_MESSAGES_IDL_ROOT}/*.idl"
)
list(SORT cpp_python_communication_idls)

set(cpp_python_communication_registrations)
set(cpp_python_communication_declarations)
set(cpp_python_communication_python_reader_specs)
set(cpp_python_communication_sources)
set(cpp_python_communication_writer_count 0)
file(MAKE_DIRECTORY "${CPP_PYTHON_COMMUNICATION_GENERATED_DIR}/writers")

foreach(idl_file IN LISTS cpp_python_communication_idls)
    file(RELATIVE_PATH idl_rel "${DLS_MESSAGES_IDL_ROOT}" "${idl_file}")
    string(REGEX REPLACE "\\.idl$" "" idl_rel_no_ext "${idl_rel}")
    string(MAKE_C_IDENTIFIER "${idl_rel_no_ext}" writer_identifier)
    set(cpp_python_communication_writer_function "add_writer_${writer_identifier}")
    set(cpp_python_communication_include "dls_messages/dds/${idl_rel_no_ext}PubSubTypes.hpp")

    file(STRINGS "${idl_file}" idl_lines)
    set(idl_modules)
    set(struct_name)

    foreach(idl_line IN LISTS idl_lines)
        if(idl_line MATCHES "^[ \t]*module[ \t]+([A-Za-z_][A-Za-z0-9_]*)[ \t]*\\{?")
            list(APPEND idl_modules "${CMAKE_MATCH_1}")
        elseif(idl_line MATCHES "^[ \t]*struct[ \t]+([A-Za-z_][A-Za-z0-9_]*)")
            set(struct_name "${CMAKE_MATCH_1}")
            break()
        endif()
    endforeach()

    if(NOT struct_name)
        message(WARNING "Skipping ${idl_rel}: no IDL struct declaration found")
        continue()
    endif()

    list(JOIN idl_modules "::" namespace_prefix)
    if(namespace_prefix)
        set(cpp_type "${namespace_prefix}::${struct_name}")
    else()
        set(cpp_type "${struct_name}")
    endif()

    get_filename_component(idl_dir "${idl_rel}" DIRECTORY)
    if(idl_dir)
        set(wrapper_module "_${writer_identifier}Wrapper")
    else()
        set(wrapper_module "_${struct_name}Wrapper")
    endif()

    string(APPEND cpp_python_communication_declarations
        "void ${cpp_python_communication_writer_function}(dls::DDSParticipant& participant, std::vector<WriterSlot>& writers);\n"
    )
    string(APPEND cpp_python_communication_registrations
        "    ${cpp_python_communication_writer_function}(participant, writers);\n"
    )
    string(APPEND cpp_python_communication_python_reader_specs
        "    MessageSpec(\"${idl_rel_no_ext}\", \"${idl_rel_no_ext}.py\", \"${struct_name}\", \"${wrapper_module}\", \"${writer_identifier}\"),\n"
    )

    set(cpp_python_communication_writer_source
        "${CPP_PYTHON_COMMUNICATION_GENERATED_DIR}/writers/${writer_identifier}.cpp"
    )
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_writer.cpp.in"
        "${cpp_python_communication_writer_source}"
        @ONLY
    )
    list(APPEND cpp_python_communication_sources "${cpp_python_communication_writer_source}")
    math(EXPR cpp_python_communication_writer_count "${cpp_python_communication_writer_count} + 1")
endforeach()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/all_idl_writers.cpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/all_idl_writers.cpp"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/python/all_idl_readers.py.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/all_idl_readers.py"
    @ONLY
)

add_executable(${CPP_PYTHON_COMMUNICATION_WRITER}
    "${CMAKE_CURRENT_BINARY_DIR}/generated/all_idl_writers.cpp"
    ${cpp_python_communication_sources}
)
target_link_libraries(${CPP_PYTHON_COMMUNICATION_WRITER}
    dls_com
    dls_messages
)
target_include_directories(${CPP_PYTHON_COMMUNICATION_WRITER}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
