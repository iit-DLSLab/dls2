# ==============================================================================
# Generation of messages
# ==============================================================================

function(dls_add_message msg library_name)
	function(_append_target_include_dirs target_name dependency_name)
		if(TARGET ${dependency_name})
			get_target_property(_dependency_include_dirs ${dependency_name} INTERFACE_INCLUDE_DIRECTORIES)
			if(_dependency_include_dirs AND NOT _dependency_include_dirs STREQUAL "_dependency_include_dirs-NOTFOUND")
				target_include_directories(${target_name}
					PRIVATE
						${_dependency_include_dirs}
				)
			endif()
		endif()
	endfunction()

	set(MESSAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/dls_messages/dds")
	set(IDL_DIR "${CMAKE_CURRENT_LIST_DIR}/../idls")
	set(IDL_FILE "${IDL_DIR}/${msg}.idl")

	set(generated_source
		"${MESSAGE_DIR}/${msg}PubSubTypes.cxx"
		"${MESSAGE_DIR}/${msg}TypeObjectSupport.cxx"
	)

	set(generated_headers
		"${MESSAGE_DIR}/${msg}.hpp"
		"${MESSAGE_DIR}/${msg}PubSubTypes.hpp"
		"${MESSAGE_DIR}/${msg}TypeObjectSupport.hpp"
		"${MESSAGE_DIR}/${msg}CdrAux.hpp"
		"${MESSAGE_DIR}/${msg}CdrAux.ipp"
	)

	set(generated_python
		"${MESSAGE_DIR}/${msg}.i"
		"${MESSAGE_DIR}/${msg}.py"
		"${MESSAGE_DIR}/${msg}PubSubTypes.i"
	)

	set(message_dependencies
		"${IDL_FILE}"
		"${CMAKE_CURRENT_LIST_DIR}/postprocess_swig_interface.cmake"
	)

	file(STRINGS "${IDL_FILE}" idl_includes REGEX "^#include \".+\\.idl\"")
	foreach(idl_include IN LISTS idl_includes)
		string(REGEX REPLACE "^#include \"([^\"]+)\\.idl\"$" "\\1" dependency "${idl_include}")
		list(APPEND message_dependencies
			"${IDL_DIR}/${dependency}.idl"
		)
	endforeach()

	find_program(FASTDDSGEN_PREPROCESSOR
		NAMES cpp clang-cpp gcc-cpp
	)

	set(fastddsgen_command
		fastddsgen
		-typeros2
		-replace
		-cs
		-python
		-I ${IDL_DIR}
	)

	if(FASTDDSGEN_PREPROCESSOR)
		list(APPEND fastddsgen_command
			-ppPath ${FASTDDSGEN_PREPROCESSOR}
		)
	endif()

	list(APPEND fastddsgen_command
		${IDL_FILE}
		-d ${MESSAGE_DIR}
	)

	add_custom_command(
		OUTPUT
			${generated_source}
			${generated_headers}
			${generated_python}
		COMMAND
			${CMAKE_COMMAND} -E make_directory ${MESSAGE_DIR}
		COMMAND
			${fastddsgen_command}
		COMMAND
			${CMAKE_COMMAND}
				-DINPUT_FILE=${MESSAGE_DIR}/${msg}.i
				-P ${CMAKE_CURRENT_LIST_DIR}/postprocess_swig_interface.cmake
		COMMENT
			"Generating C++ and Python message files for ${msg}.idl"
		DEPENDS
			${message_dependencies}
		VERBATIM
	)

	target_sources(${library_name}
		PRIVATE
			${generated_source}
	)

	target_include_directories(${library_name}
		PUBLIC
			${CMAKE_CURRENT_BINARY_DIR}/include
	)

	target_link_libraries(${library_name}
		PUBLIC
			fastcdr
			fastdds
	)

	foreach(idl_include IN LISTS idl_includes)
		string(REGEX REPLACE "^#include \"([^\"]+)\\.idl\"$" "\\1" dependency "${idl_include}")

		if(TARGET "${dependency}")
			target_link_libraries(${library_name}
				PUBLIC
					${dependency}
			)
		endif()
	endforeach()

	find_package(SWIG)
	if(NOT SWIG_FOUND)
		find_program(SWIG_EXECUTABLE NAMES swig4.1 swig)
		find_package(SWIG REQUIRED)
	endif()

	include(${SWIG_USE_FILE})

	find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

	set(${msg}_MODULE ${msg}Wrapper)
	set(${msg}_MODULE_FILES "${MESSAGE_DIR}/${msg}.i")

	set_source_files_properties(
		${${msg}_MODULE_FILES}
		PROPERTIES
			CPLUSPLUS ON
			USE_TARGET_INCLUDE_DIRECTORIES TRUE
			SWIG_COMPILE_OPTIONS "-w509"
	)

	set_property(
		SOURCE "${MESSAGE_DIR}/${msg}.i"
		PROPERTY OUTPUT_DIR "${MESSAGE_DIR}"
	)

	swig_add_library(${${msg}_MODULE}
		TYPE SHARED
		LANGUAGE python
		SOURCES ${${msg}_MODULE_FILES}
	)

	add_dependencies(${${msg}_MODULE} ${library_name})

	set_property(TARGET ${${msg}_MODULE} PROPERTY CXX_STANDARD 11)

	# SWIG-generated Python wrappers use the CPython callback signature
	# `PyObject *self, PyObject *args`, but some generated entry points do not
	# consume `self`. Keep strict warnings for handwritten code and suppress
	# only generator-related warnings on the generated wrapper target.
	target_compile_options(${${msg}_MODULE}
		PRIVATE
			$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-unused-parameter>
			$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-missing-field-initializers>
			$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-delete-non-virtual-dtor>
			$<$<CXX_COMPILER_ID:MSVC>:/wd4100>
	)

	if(UNIX AND CMAKE_SIZEOF_VOID_P EQUAL 8)
		set_property(
			TARGET ${${msg}_MODULE}
			PROPERTY SWIG_COMPILE_DEFINITIONS SWIGWORDSIZE64
		)
	endif()

	target_include_directories(${${msg}_MODULE}
		PRIVATE
			${CMAKE_CURRENT_BINARY_DIR}/include
			${MESSAGE_DIR}
			/usr/local/include
	)

	target_link_libraries(${${msg}_MODULE}
		PRIVATE
			Python3::Module
			fastdds
			fastcdr
			${library_name}
	)

	set_target_properties(${${msg}_MODULE}
		PROPERTIES
			LIBRARY_OUTPUT_DIRECTORY "${MESSAGE_DIR}"
	)

	execute_process(
		COMMAND
			${Python3_EXECUTABLE} -c "import sysconfig; schemes = sysconfig.get_scheme_names(); scheme = 'deb_system' if 'deb_system' in schemes else sysconfig.get_default_scheme(); print(sysconfig.get_path('purelib', scheme=scheme))"
		OUTPUT_VARIABLE
			_ABS_PYTHON_MODULE_PATH
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	get_filename_component(_ABS_PYTHON_MODULE_PATH ${_ABS_PYTHON_MODULE_PATH} ABSOLUTE)
	file(RELATIVE_PATH _REL_PYTHON_MODULE_PATH ${CMAKE_INSTALL_PREFIX} ${_ABS_PYTHON_MODULE_PATH})
	set(PYTHON_MODULE_PATH ${_REL_PYTHON_MODULE_PATH})

	install(
		FILES
			${generated_headers}
		DESTINATION
			/usr/include/dls_messages/dds
		COMPONENT
			${PROJECT_NAME}_dev
	)

	install(
		TARGETS
			${${msg}_MODULE}
		DESTINATION
			${PYTHON_MODULE_PATH}
	)

	get_property(
		support_files
		TARGET ${${msg}_MODULE}
		PROPERTY SWIG_SUPPORT_FILES
	)

	install(
		FILES
			${support_files}
			${MESSAGE_DIR}/${msg}.py
		DESTINATION
			${PYTHON_MODULE_PATH}
	)
endfunction()
