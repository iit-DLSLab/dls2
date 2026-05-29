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

file(WRITE "${INPUT_FILE}" "${interface_contents}")
