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

#################################################################################
function(set_msg_build_names idl_file_name subdirectory)
    # set message directory for generated files
    set(MESSAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/dls_messages/dds${subdirectory}")

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
    # set library name for the generated cpp files
    string(REPLACE "/" "_" subdirectory_name "${subdirectory}")
    string(REGEX REPLACE "^_" "" subdirectory_name "${subdirectory_name}")
    set(subdirectory_name ${subdirectory_name})

    set(CPP_LIBRARY_NAME "${subdirectory_name}_${idl_file_name}_msg_cpp")

    # set global properties for the generated files and library name
    set_property(GLOBAL PROPERTY MESSAGE_DIR "${MESSAGE_DIR}")
    set_property(GLOBAL PROPERTY generated_cpp_source "${generated_cpp_source}")
    set_property(GLOBAL PROPERTY generated_cpp_headers "${generated_cpp_headers}")
    set_property(GLOBAL PROPERTY generated_py_source "${generated_py_source}")
    set_property(GLOBAL PROPERTY subdirectory_name "${subdirectory_name}")
    set_property(GLOBAL PROPERTY CPP_LIBRARY_NAME "${CPP_LIBRARY_NAME}")

endfunction()
#################################################################################

#################################################################################
function(extract_file_and_subdirectory_names idl_file)
    get_filename_component(idl_file_name "${idl_file}" NAME_WE)
    get_filename_component(idl_directory "${idl_file}" DIRECTORY)

    file(RELATIVE_PATH subdirectory
        "${CMAKE_CURRENT_SOURCE_DIR}/idls"
        "${idl_directory}"
    )
    if(subdirectory STREQUAL ".")
        set(subdirectory "")
    else()
        set(subdirectory "/${subdirectory}")
    endif()
    # set global properties for the extracted names
    set_property(GLOBAL PROPERTY idl_file_name "${idl_file_name}")
    set_property(GLOBAL PROPERTY subdirectory "${subdirectory}")
endfunction()
#################################################################################

function(fastddsgen_trigger idl_file_path)
    extract_file_and_subdirectory_names(${idl_file_path})

    get_property(idl_file_name GLOBAL PROPERTY idl_file_name)
    get_property(subdirectory GLOBAL PROPERTY subdirectory)
    set_msg_build_names("${idl_file_name}" "${subdirectory}")

    ############################################################
    # RUN FASTDDSGEN TO GENERATE CPP AND PYTHON FILES
    get_property(MESSAGE_DIR GLOBAL PROPERTY MESSAGE_DIR)
    set(fastddsgen_command
        fastddsgen
        -typeros2
        -replace
        -cs
        -python
        -d
        ${MESSAGE_DIR}
        -I ${CMAKE_CURRENT_SOURCE_DIR}/idls
        -I ${CMAKE_CURRENT_SOURCE_DIR}/idls/ros2_interface
        ${idl_file_path}
	)

    get_property(generated_cpp_source GLOBAL PROPERTY generated_cpp_source)
    get_property(generated_cpp_headers GLOBAL PROPERTY generated_cpp_headers)
    get_property(generated_py_source GLOBAL PROPERTY generated_py_source)
    add_custom_command(
		OUTPUT "${MESSAGE_DIR}/${idl_file_name}.stamp"
        BYPRODUCTS
			${generated_cpp_source}
			${generated_cpp_headers}
            ${generated_py_source}
		COMMAND
			${CMAKE_COMMAND} -E make_directory ${MESSAGE_DIR}
		COMMAND
            ${fastddsgen_command}
		COMMAND
			${CMAKE_COMMAND} -E touch "${MESSAGE_DIR}/${idl_file_name}.stamp"
		COMMENT
			"Generating message files for ${idl_file_name}.idl"
		DEPENDS
		${idl_file_path}
	)
    # ############################################################

    # ############################################################
    # Create a custom target to ensure that the generated files are created before building the cpp/python libraries
    get_property(CPP_LIBRARY_NAME GLOBAL PROPERTY CPP_LIBRARY_NAME)
    add_custom_target(${CPP_LIBRARY_NAME}_target ALL
        DEPENDS 
            "${MESSAGE_DIR}/${idl_file_name}.stamp"
    )
    # Chaining ${CPP_LIBRARY_NAME}_target generator targets sequentially, so fastddsgen -replace cannot run concurrently for different IDLs.
    get_property(previous_fastddsgen_target GLOBAL PROPERTY DLS_MESSAGES_PREVIOUS_FASTDDSGEN_TARGET)
    if(previous_fastddsgen_target)
        add_dependencies(${CPP_LIBRARY_NAME}_target ${previous_fastddsgen_target})
    endif()
    set_property(GLOBAL PROPERTY DLS_MESSAGES_PREVIOUS_FASTDDSGEN_TARGET ${CPP_LIBRARY_NAME}_target)
    # Define a variable to hold all the fastddsgen targets, to wait their compilation before cpp and python libraries are built.
    set(DLS_MESSAGES_FASTDDSGEN_TARGETS
        ${DLS_MESSAGES_FASTDDSGEN_TARGETS}
        ${CPP_LIBRARY_NAME}_target
        PARENT_SCOPE
    )

endfunction()

function(generate_msg_library idl_file_path library_name)   
    extract_file_and_subdirectory_names(${idl_file_path})

    get_property(idl_file_name GLOBAL PROPERTY idl_file_name)
    get_property(subdirectory GLOBAL PROPERTY subdirectory)
    set_msg_build_names("${idl_file_name}" "${subdirectory}")

    # ############################################################
    # add CPP source files to the library and include directories for the library
    get_property(CPP_LIBRARY_NAME GLOBAL PROPERTY CPP_LIBRARY_NAME)
    get_property(generated_cpp_source GLOBAL PROPERTY generated_cpp_source)
    get_property(generated_cpp_headers GLOBAL PROPERTY generated_cpp_headers)

    target_sources(${library_name}
		PRIVATE
			${generated_cpp_source}
	)
    # add dependency to ensure that the generated files are created before building the cpp library

    get_property(MESSAGE_DIR GLOBAL PROPERTY MESSAGE_DIR)
    target_include_directories(${library_name}
        PUBLIC
            ${MESSAGE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/include/dls_messages/dds/ros2_interface
    )
    # ############################################################

    # ############################################################
    # add PYTHON bindings for the generated source files
    # from FAST-DDS-python example
    get_property(subdirectory_name GLOBAL PROPERTY subdirectory_name)
    # if subdiredtory_name is empty, set ${idl_file_name}Wrapper instaead of _${subdirectory_name}_${idl_file_name}Wrapper
    if(subdirectory_name STREQUAL "")
        set(${idl_file_name}_MODULE "${idl_file_name}Wrapper")
    else()
        set(${idl_file_name}_MODULE "${subdirectory_name}_${idl_file_name}Wrapper")
    endif()

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
        )

    SWIG_ADD_LIBRARY(${${idl_file_name}_MODULE}
        TYPE SHARED
        LANGUAGE python
        SOURCES ${${idl_file_name}_MODULE_FILES})

    # add dependency to ensure that the generated files are created before building the python library
    add_dependencies(${${idl_file_name}_MODULE}
            dls_messages_fastddsgen_all
    )
    #  ${${idl_file_name}_MODULE}_swig_compilation target can be created automatically by CMake
    if(TARGET ${${idl_file_name}_MODULE}_swig_compilation)
        add_dependencies(${${idl_file_name}_MODULE}_swig_compilation
            dls_messages_fastddsgen_all
        )
    endif()

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
        ${library_name}
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

    ############################################################
    # Install python bindings
    # Find the installation path
 	execute_process(
		COMMAND
			${Python3_EXECUTABLE} -c "import sysconfig; schemes = sysconfig.get_scheme_names(); scheme = 'deb_system' if 'deb_system' in schemes else sysconfig.get_default_scheme(); print(sysconfig.get_path('purelib', scheme=scheme))"
		OUTPUT_VARIABLE
			_ABS_PYTHON_MODULE_PATH
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	get_filename_component(_ABS_PYTHON_MODULE_PATH ${_ABS_PYTHON_MODULE_PATH} ABSOLUTE)
    file (RELATIVE_PATH _REL_PYTHON_MODULE_PATH ${CMAKE_INSTALL_PREFIX} ${_ABS_PYTHON_MODULE_PATH})
    SET (PYTHON_MODULE_PATH
        ${_REL_PYTHON_MODULE_PATH}/${PROJECT_NAME}
        )
    install(TARGETS ${${idl_file_name}_MODULE} DESTINATION ${PYTHON_MODULE_PATH})
    get_property(support_files TARGET ${${idl_file_name}_MODULE} PROPERTY SWIG_SUPPORT_FILES)
    install(FILES ${support_files} DESTINATION ${PYTHON_MODULE_PATH})
endfunction()