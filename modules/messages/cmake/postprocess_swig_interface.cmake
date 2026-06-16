if(NOT DEFINED INPUT_FILE)
    message(FATAL_ERROR "INPUT_FILE must be provided")
endif()

if(NOT EXISTS "${INPUT_FILE}")
    message(FATAL_ERROR "SWIG interface file not found: ${INPUT_FILE}")
endif()

function(_dls_postprocess_swig_interface input_file)
    if(NOT EXISTS "${input_file}")
        return()
    endif()

    get_filename_component(input_file_abs "${input_file}" ABSOLUTE)
    get_property(processed_interfaces GLOBAL PROPERTY DLS_SWIG_POSTPROCESSED_FILES)
    if(processed_interfaces)
        list(FIND processed_interfaces "${input_file_abs}" processed_index)
        if(NOT processed_index EQUAL -1)
            return()
        endif()
    endif()
    set_property(GLOBAL APPEND PROPERTY DLS_SWIG_POSTPROCESSED_FILES "${input_file_abs}")

    file(READ "${input_file}" interface_contents)
    get_filename_component(interface_dir "${input_file}" DIRECTORY)
    get_filename_component(interface_name "${input_file}" NAME_WE)

    string(REGEX MATCHALL "%include \"[^\"]+\\.i\"" included_interfaces "${interface_contents}")

    # Included interfaces are expanded by SWIG inside the current wrapper, so
    # patch them first. This matters for transitive types such as Pose ->
    # Quaternion when another wrapper includes Pose.i.
    foreach(included_interface IN LISTS included_interfaces)
        string(REGEX REPLACE "^%include \"([^\"]+)\\.i\"$" "\\1" included_name "${included_interface}")
        get_filename_component(included_type_name "${included_name}" NAME_WE)

        if(included_type_name STREQUAL "${interface_name}" OR included_type_name MATCHES "PubSubTypes$")
            continue()
        endif()

        set(included_interface_file "${interface_dir}/${included_name}.i")
        if(EXISTS "${included_interface_file}")
            _dls_postprocess_swig_interface("${included_interface_file}")
        endif()
    endforeach()

    file(READ "${input_file}" interface_contents)

    # Silence SWIG warnings directly in the interface file. Insert the filter
    # immediately after `%module`, which is reliably honored by SWIG.
    #
    # 302: duplicate template alias definitions like `double_vector`.
    # 389: operator[] ignored in Fast DDS Loanable* container internals.
    # 401: base class unknown after dropping the internal LoanableTypedCollection
    #      template instantiation to avoid bad SWIG-generated destructors.
    # 509: overloaded methods shadowed during Python binding generation.
    if(NOT interface_contents MATCHES "%warnfilter\\(302,389,401,509\\);")
        string(REGEX REPLACE
            "(%module\\([^\n]*\\) [^\n]+)"
            "\\1\n%warnfilter(302,389,401,509);"
            interface_contents
            "${interface_contents}"
        )
    endif()

    # Fast DDS generates an internal LoanableTypedCollection alias for each
    # sequence. SWIG then emits Python-owned destructors for that abstract base
    # type, producing delete-non-virtual-dtor warnings in generated wrappers.
    string(REGEX REPLACE
        "(^|\n)%template\\(_[A-Za-z0-9_]+Seq\\) eprosima::fastdds::dds::LoanableTypedCollection<[^;]+>;"
        "\\1"
        interface_contents
        "${interface_contents}"
    )

    # fastddsgen writes accessor signatures with short type names when a field
    # type comes from an included IDL, while the generated C++ class itself
    # lives in the IDL namespace. Teach SWIG and the C++ wrapper about the same
    # short aliases before the main header is parsed.
    if(NOT interface_contents MATCHES "DLS_SWIG_INCLUDED_TYPE_ALIASES")
        set(swig_type_aliases "")
        set(cpp_type_aliases "")

        foreach(included_interface IN LISTS included_interfaces)
            string(REGEX REPLACE "^%include \"([^\"]+)\\.i\"$" "\\1" included_name "${included_interface}")
            get_filename_component(included_type_name "${included_name}" NAME_WE)

            if(included_type_name STREQUAL "${interface_name}" OR included_type_name MATCHES "PubSubTypes$")
                continue()
            endif()

            set(included_interface_file "${interface_dir}/${included_name}.i")
            if(EXISTS "${included_interface_file}")
                file(READ "${included_interface_file}" included_interface_contents)
                string(REGEX MATCH
                    "Binding for class ([A-Za-z_][A-Za-z0-9_:]*::${included_type_name})"
                    included_class_match
                    "${included_interface_contents}"
                )

                if(included_class_match)
                    set(included_class "${CMAKE_MATCH_1}")
                    string(APPEND swig_type_aliases "typedef ${included_class} ${included_type_name};\n")
                    string(APPEND cpp_type_aliases "using ${included_type_name} = ${included_class};\n")
                endif()
            endif()
        endforeach()

        if(swig_type_aliases)
            set(type_alias_block
                "/* DLS_SWIG_INCLUDED_TYPE_ALIASES */\n%{\n${cpp_type_aliases}%}\n${swig_type_aliases}\n"
            )

            if(interface_contents MATCHES "\n////////////////////////////////////////////////////////\n// Binding for class")
                string(REGEX REPLACE
                    "\n(////////////////////////////////////////////////////////\n// Binding for class)"
                    "\n${type_alias_block}\n\\1"
                    interface_contents
                    "${interface_contents}"
                )
            else()
                string(REGEX REPLACE
                    "\n(%include \"${interface_name}\\.hpp\")"
                    "\n${type_alias_block}\n\\1"
                    interface_contents
                    "${interface_contents}"
                )
            endif()
        endif()
    endif()

    file(WRITE "${input_file}" "${interface_contents}")
endfunction()

_dls_postprocess_swig_interface("${INPUT_FILE}")
