################################################################################
# INITIALIZATION FOR PYTHON BINDINGS
# find swig for python bindings
find_package(SWIG)
if (NOT SWIG_FOUND)
    # Trick to find swig4.1 in Ubuntu noble.
    find_program(SWIG_EXECUTABLE NAMES swig4.1 swig)
    find_package(SWIG REQUIRED)
endif()
include(${SWIG_USE_FILE})
set(CMAKE_SWIG_FLAGS "")

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
################################################################################

function(generate_msg_library idl_file_path idl_file_name)
    # set message directory for generated files
    set(MESSAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/dls_messages/dds")
    
    ############################################################
    # RUN FASTDDSGEN TO GENERATE CPP AND PYTHON FILES
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
    ############################################################

    ############################################################
    # add CPP library for the generated source files
    set (CPP_LIBRARY_NAME "${idl_file_name}_msg_cpp")
    add_library(${CPP_LIBRARY_NAME} SHARED
        ${generated_cpp_source}
        )
    target_include_directories(${CPP_LIBRARY_NAME}
        PUBLIC
            ${MESSAGE_DIR}
    )
    target_link_libraries(${CPP_LIBRARY_NAME}
        PUBLIC
            fastcdr
            fastdds
    )
    ############################################################

    ############################################################
    # add PYTHON bindings for the generated source files
    # from FAST-DDS-python example
    set(${idl_file_name}_MODULE
        ${idl_file_name}Wrapper
        )

    set(${idl_file_name}_MODULE_FILES
        ${MESSAGE_DIR}/${idl_file_name}.i
        )
    # -w302: Warning 302: Identifier 'double_vector' redefined (ignored) (Renamed from 'vector< double >'),
    # -w389: Warning 389: /usr/local/include/fastdds/dds/core/LoanableTypedCollection.hpp:68: Warning 389: operator[] ignored (consider using %extend)
    # -w509: Warning 509: Warning 509: Overloaded method dls2_interface::msg::ArmState::ArmState(dls2_interface::msg::ArmState &&) effectively ignored,
    SET_SOURCE_FILES_PROPERTIES(
        ${${idl_file_name}_MODULE_FILES}
        PROPERTIES CPLUSPLUS ON  OUTPUT_DIR "${MESSAGE_DIR}" SWIG_FLAGS "-w302,389,509"
        USE_TARGET_INCLUDE_DIRECTORIES TRUE
        # SWIG_COMPILE_OPTIONS "-w302"
        )

    SWIG_ADD_LIBRARY(${${idl_file_name}_MODULE}
        TYPE SHARED
        LANGUAGE python
        SOURCES ${${idl_file_name}_MODULE_FILES})

    set_property(TARGET ${${idl_file_name}_MODULE} PROPERTY CXX_STANDARD 11)
    if(UNIX AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        set_property(TARGET ${${idl_file_name}_MODULE} PROPERTY SWIG_COMPILE_DEFINITIONS SWIGWORDSIZE64)
    endif()
    target_include_directories(${${idl_file_name}_MODULE}
		PRIVATE
			/usr/local/include
	)

    target_link_libraries(${${idl_file_name}_MODULE}
        Python3::Module
        fastdds
        ${CPP_LIBRARY_NAME}
        )
    # -Wno-missing-field-initializers: warning: missing initializer for member '_typeobject::tp_watched' 
    # -Wno-unused-parameter: warning: unused parameter 'self' [-Wunused-parameter] 5496 | SWIGINTERN PyObject *_wrap_delete_SwigPyIterator(PyObject *self, PyObject *args)
    # -Wno-delete-non-virtual-dtor: warning: deleting object of abstract class type 'eprosima::fastdds::dds::LoanableTypedCollection<dls2_interface::msg::ArmState, std::integral_constant<bool, false> >' which has non-virtual destructor will cause undefined behavior [-Wdelete-non-virtual-dtor] 10017 |   delete arg1;
    target_compile_options(${${idl_file_name}_MODULE} PRIVATE
        -Wno-missing-field-initializers
        -Wno-unused-parameter
        -Wno-delete-non-virtual-dtor
    )
    ############################################################
endfunction()