if(NOT DEFINED INPUT_FILE)
    message(FATAL_ERROR "INPUT_FILE must be provided")
endif()

if(NOT EXISTS "${INPUT_FILE}")
    message(FATAL_ERROR "SWIG interface file not found: ${INPUT_FILE}")
endif()

file(READ "${INPUT_FILE}" interface_contents)

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

# Fast DDS generates an internal LoanableTypedCollection alias for each sequence:
#   %template(_FooSeq) LoanableTypedCollection<...>;
# SWIG then emits Python-owned destructors for that abstract base type, producing
# delete-non-virtual-dtor warnings in the generated *_wrap.cxx files.
# Keep the concrete LoanableSequence binding and drop only the internal alias.
string(REGEX REPLACE
    "(^|\n)%template\\(_[A-Za-z0-9_]+Seq\\) eprosima::fastdds::dds::LoanableTypedCollection<[^;]+>;"
    "\\1"
    interface_contents
    "${interface_contents}"
)

# fastddsgen writes accessor signatures with short type names when a field type
# comes from an included IDL, while the generated C++ class itself lives in the
# IDL namespace, e.g. dls2_interface::msg::DlsHeader. Teach SWIG and the C++
# wrapper about the same short aliases before the main header is parsed.
if(NOT interface_contents MATCHES "DLS_SWIG_INCLUDED_TYPE_ALIASES")
    get_filename_component(interface_dir "${INPUT_FILE}" DIRECTORY)
    get_filename_component(interface_name "${INPUT_FILE}" NAME_WE)
    set(swig_type_aliases "")
    set(cpp_type_aliases "")

    string(REGEX MATCHALL "%include \"[^\"]+\\.i\"" included_interfaces "${interface_contents}")
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

        string(REGEX REPLACE
            "\n(%include \"${interface_name}\\.hpp\")"
            "\n${type_alias_block}\n\\1"
            interface_contents
            "${interface_contents}"
        )
    endif()
endif()

file(WRITE "${INPUT_FILE}" "${interface_contents}")
