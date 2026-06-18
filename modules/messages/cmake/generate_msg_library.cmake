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

function(fastddsgen_trigger idl_file_path idl_file_name subdirectory)
# set message directory for generated files
    set(MESSAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/dls_messages/dds${subdirectory}")
    
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
        -d
        ${MESSAGE_DIR}
        -I ${CMAKE_CURRENT_SOURCE_DIR}/idls
        -I ${CMAKE_CURRENT_SOURCE_DIR}/idls/ros2_interface
        ${idl_file_path}
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
    # ############################################################

    # ############################################################
    # add CPP library for the generated source files
    # extract subdirectory name from subdirectory, subsittuting "/" with "_" and removing leading "_"
    string(REPLACE "/" "_" subdirectory_name "${subdirectory}")
    string(REGEX REPLACE "^_" "" subdirectory_name "${subdirectory_name}")
    set(CPP_LIBRARY_NAME "${subdirectory_name}_${idl_file_name}_msg_cpp")
    # message (STATUS "Generating CPP library ${CPP_LIBRARY_NAME} for ${idl_file_name}.idl in subdirectory ${subdirectory}")

    add_custom_target(${CPP_LIBRARY_NAME}_target ALL
        DEPENDS 
			${generated_cpp_source}
			${generated_cpp_headers}
            ${generated_py_source}
    )

endfunction()

function(generate_msg_library idl_file_path idl_file_name subdirectory)   
    # set message directory for generated files
    set(MESSAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/dls_messages/dds${subdirectory}") 
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
    # ############################################################

    # ############################################################
    # add CPP library for the generated source files
    # extract subdirectory name from subdirectory, subsittuting "/" with "_" and removing leading "_"
    string(REPLACE "/" "_" subdirectory_name "${subdirectory}")
    string(REGEX REPLACE "^_" "" subdirectory_name "${subdirectory_name}")
    set(CPP_LIBRARY_NAME "${subdirectory_name}_${idl_file_name}_msg_cpp")
    # message (STATUS "Generating CPP library ${CPP_LIBRARY_NAME} for ${idl_file_name}.idl in subdirectory ${subdirectory}")
    add_library(${CPP_LIBRARY_NAME} SHARED
        ${generated_cpp_source}
        )
    add_dependencies(${CPP_LIBRARY_NAME}
        ${CPP_LIBRARY_NAME}_target
    )   
    target_include_directories(${CPP_LIBRARY_NAME}
        PUBLIC
            ${MESSAGE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/include/dls_messages/dds/ros2_interface
    )
    target_link_libraries(${CPP_LIBRARY_NAME}
        PUBLIC
            fastcdr
            fastdds
    )
    
    # ############################################################

    # ############################################################
    # add PYTHON bindings for the generated source files
    # from FAST-DDS-python example
    set(${idl_file_name}_MODULE
        ${subdirectory_name}_${idl_file_name}Wrapper
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
    add_dependencies(${${idl_file_name}_MODULE}
        ${CPP_LIBRARY_NAME}_target
    )
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

    ############################################################
    # Install cpp library and headers
    install(TARGETS ${CPP_LIBRARY_NAME}
        DESTINATION ${DLS_INSTALL_MESSAGES_DIR}/${subdirectory}
        COMPONENT ${PROJECT_NAME}_dev
    )
    install (
        FILES ${generated_cpp_headers}
        DESTINATION ${DLS_INSTALL_MESSAGES_HEADER_DIR}/${subdirectory}
        COMPONENT ${PROJECT_NAME}_dev
    )
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