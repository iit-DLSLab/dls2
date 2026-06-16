# ==============================================================================
# Generation of messages
# ==============================================================================

function(dls_add_message msg library_name)

	set(IDL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/idls")
	set(MESSAGE_ROOT "${CMAKE_CURRENT_BINARY_DIR}/include/dls_messages/dds")

	file(GLOB_RECURSE all_idl_files CONFIGURE_DEPENDS "${IDL_ROOT}/*.idl")

	if(IS_ABSOLUTE "${msg}")
		set(IDL_FILE "${msg}")
		file(RELATIVE_PATH idl_rel "${IDL_ROOT}" "${IDL_FILE}")
	elseif("${msg}" MATCHES "\\.idl$")
		set(idl_rel "${msg}")
		set(IDL_FILE "${IDL_ROOT}/${idl_rel}")
	else()
		set(idl_rel "${msg}.idl")
		set(IDL_FILE "${IDL_ROOT}/${idl_rel}")

		if(NOT EXISTS "${IDL_FILE}")
			set(idl_matches)
			foreach(candidate_idl IN LISTS all_idl_files)
				get_filename_component(candidate_name "${candidate_idl}" NAME_WE)
				if(candidate_name STREQUAL "${msg}")
					list(APPEND idl_matches "${candidate_idl}")
				endif()
			endforeach()

			list(LENGTH idl_matches idl_match_count)
			if(idl_match_count EQUAL 1)
				list(GET idl_matches 0 IDL_FILE)
				file(RELATIVE_PATH idl_rel "${IDL_ROOT}" "${IDL_FILE}")
			elseif(idl_match_count GREATER 1)
				message(FATAL_ERROR "Message '${msg}' is ambiguous. Pass the IDL path relative to ${IDL_ROOT}.")
			endif()
		endif()
	endif()

	if(NOT EXISTS "${IDL_FILE}")
		message(FATAL_ERROR "IDL file not found: ${IDL_FILE}")
	endif()

	get_filename_component(msg_name "${IDL_FILE}" NAME_WE)
	get_filename_component(idl_dir "${idl_rel}" DIRECTORY)
	string(REGEX REPLACE "\\.idl$" "" idl_rel_no_ext "${idl_rel}")
	string(MAKE_C_IDENTIFIER "${idl_rel_no_ext}" msg_identifier)
	set(message_generation_target "${msg_identifier}_generate")

	if(idl_dir)
		set(MESSAGE_DIR "${MESSAGE_ROOT}/${idl_dir}")
		set(install_dir "/usr/include/dls_messages/dds/${idl_dir}")
	else()
		set(MESSAGE_DIR "${MESSAGE_ROOT}")
		set(install_dir "/usr/include/dls_messages/dds")
	endif()

	set(generated_source
		"${MESSAGE_DIR}/${msg_name}PubSubTypes.cxx"
		"${MESSAGE_DIR}/${msg_name}TypeObjectSupport.cxx"
	)

	set(generated_headers
		"${MESSAGE_DIR}/${msg_name}.hpp"
		"${MESSAGE_DIR}/${msg_name}PubSubTypes.hpp"
		"${MESSAGE_DIR}/${msg_name}TypeObjectSupport.hpp"
		"${MESSAGE_DIR}/${msg_name}CdrAux.hpp"
		"${MESSAGE_DIR}/${msg_name}CdrAux.ipp"
	)

	set(generated_python
		"${MESSAGE_DIR}/${msg_name}.i"
		"${MESSAGE_DIR}/${msg_name}.py"
		"${MESSAGE_DIR}/${msg_name}PubSubTypes.i"
	)

	set(message_dependencies
		"${IDL_FILE}"
		"${CMAKE_CURRENT_LIST_DIR}/postprocess_swig_interface.cmake"
	)

	file(STRINGS "${IDL_FILE}" idl_includes REGEX "^#include \".+\\.idl\"")
	foreach(idl_include IN LISTS idl_includes)
		string(REGEX REPLACE "^#include \"([^\"]+\\.idl)\"$" "\\1" dependency "${idl_include}")
		set(dependency_file "")

		foreach(candidate_idl IN LISTS all_idl_files)
			file(RELATIVE_PATH candidate_rel "${IDL_ROOT}" "${candidate_idl}")
			if(candidate_rel STREQUAL "${dependency}" OR candidate_rel MATCHES "/${dependency}$")
				set(dependency_file "${candidate_idl}")
				break()
			endif()
		endforeach()

		if(dependency_file)
			list(APPEND message_dependencies "${dependency_file}")
		endif()
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
	)

	set(idl_include_dirs "${IDL_ROOT}")
	if(EXISTS "${IDL_ROOT}/ros2_interface")
		list(APPEND idl_include_dirs "${IDL_ROOT}/ros2_interface")
	endif()

	foreach(candidate_idl IN LISTS all_idl_files)
		get_filename_component(candidate_idl_dir "${candidate_idl}" DIRECTORY)
		list(APPEND idl_include_dirs "${candidate_idl_dir}")
	endforeach()
	list(REMOVE_DUPLICATES idl_include_dirs)

	foreach(idl_include_dir IN LISTS idl_include_dirs)
		list(APPEND fastddsgen_command -I "${idl_include_dir}")
	endforeach()

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
				-DINPUT_FILE=${MESSAGE_DIR}/${msg_name}.i
				-P ${CMAKE_CURRENT_LIST_DIR}/postprocess_swig_interface.cmake
		COMMENT
			"Generating C++ and Python message files for ${idl_rel}"
		DEPENDS
			${message_dependencies}
		VERBATIM
	)

	add_custom_target(${message_generation_target}
		DEPENDS
			${generated_source}
			${generated_headers}
			${generated_python}
	)

	foreach(idl_include IN LISTS idl_includes)
		string(REGEX REPLACE "^#include \"([^\"]+\\.idl)\"$" "\\1" dependency "${idl_include}")
		set(dependency_file "")

		foreach(candidate_idl IN LISTS all_idl_files)
			file(RELATIVE_PATH candidate_rel "${IDL_ROOT}" "${candidate_idl}")
			if(candidate_rel STREQUAL "${dependency}" OR candidate_rel MATCHES "/${dependency}$")
				set(dependency_file "${candidate_idl}")
				break()
			endif()
		endforeach()

		if(dependency_file)
			file(RELATIVE_PATH dependency_rel "${IDL_ROOT}" "${dependency_file}")
			string(REGEX REPLACE "\\.idl$" "" dependency_rel_no_ext "${dependency_rel}")
			string(MAKE_C_IDENTIFIER "${dependency_rel_no_ext}" dependency_identifier)
			set(dependency_generation_target "${dependency_identifier}_generate")

			set_property(GLOBAL APPEND PROPERTY
				DLS_MESSAGE_GENERATION_DEPENDENCIES
				"${message_generation_target}|${dependency_generation_target}"
			)

			if(TARGET "${dependency_generation_target}")
				add_dependencies(${message_generation_target}
					"${dependency_generation_target}"
				)
			endif()
		endif()
	endforeach()

	add_dependencies(${library_name}
		${message_generation_target}
	)

	target_sources(${library_name}
		PRIVATE
			${generated_source}
	)

	target_include_directories(${library_name}
		PUBLIC
			${CMAKE_CURRENT_BINARY_DIR}/include
			${MESSAGE_ROOT}
			${MESSAGE_ROOT}/ros2_interface
			${MESSAGE_DIR}
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

	set(${msg_identifier}_MODULE ${msg_identifier}Wrapper)
	set(${msg_identifier}_MODULE_FILES "${MESSAGE_DIR}/${msg_name}.i")

	set_source_files_properties(
		${${msg_identifier}_MODULE_FILES}
		PROPERTIES
			CPLUSPLUS ON
			USE_TARGET_INCLUDE_DIRECTORIES TRUE
			SWIG_COMPILE_OPTIONS "-w509"
	)

	set_property(
		SOURCE "${MESSAGE_DIR}/${msg_name}.i"
		PROPERTY OUTPUT_DIR "${MESSAGE_DIR}"
	)

	swig_add_library(${${msg_identifier}_MODULE}
		TYPE SHARED
		LANGUAGE python
		SOURCES ${${msg_identifier}_MODULE_FILES}
	)

	add_dependencies(${${msg_identifier}_MODULE}
		${library_name}
		${message_generation_target}
	)

	set_property(TARGET ${${msg_identifier}_MODULE} PROPERTY CXX_STANDARD 11)

	# SWIG-generated Python wrappers use the CPython callback signature
	# `PyObject *self, PyObject *args`, but some generated entry points do not
	# consume `self`. Keep strict warnings for handwritten code and suppress
	# only generator-related warnings on the generated wrapper target.
	target_compile_options(${${msg_identifier}_MODULE}
		PRIVATE
			$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-unused-parameter>
			$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-missing-field-initializers>
			$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-delete-non-virtual-dtor>
			$<$<CXX_COMPILER_ID:MSVC>:/wd4100>
	)

	if(UNIX AND CMAKE_SIZEOF_VOID_P EQUAL 8)
		set_property(
			TARGET ${${msg_identifier}_MODULE}
			PROPERTY SWIG_COMPILE_DEFINITIONS SWIGWORDSIZE64
		)
	endif()

	target_include_directories(${${msg_identifier}_MODULE}
		PRIVATE
			${CMAKE_CURRENT_BINARY_DIR}/include
			${MESSAGE_ROOT}
			${MESSAGE_ROOT}/ros2_interface
			${MESSAGE_DIR}
			/usr/local/include
	)

	target_link_libraries(${${msg_identifier}_MODULE}
		PRIVATE
			Python3::Module
			fastdds
			fastcdr
			${library_name}
	)

	set_target_properties(${${msg_identifier}_MODULE}
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
			${install_dir}
		COMPONENT
			${PROJECT_NAME}_dev
	)

	install(
		TARGETS
			${${msg_identifier}_MODULE}
		DESTINATION
			${PYTHON_MODULE_PATH}
	)

	get_property(
		support_files
		TARGET ${${msg_identifier}_MODULE}
		PROPERTY SWIG_SUPPORT_FILES
	)

	install(
		FILES
			${support_files}
			${MESSAGE_DIR}/${msg_name}.py
		DESTINATION
			${PYTHON_MODULE_PATH}
	)
endfunction()

function(dls_finalize_messages)
	get_property(message_generation_dependencies GLOBAL PROPERTY DLS_MESSAGE_GENERATION_DEPENDENCIES)
	foreach(message_dependency IN LISTS message_generation_dependencies)
		string(REPLACE "|" ";" dependency_pair "${message_dependency}")
		list(GET dependency_pair 0 message_generation_target)
		list(GET dependency_pair 1 dependency_generation_target)

		if(TARGET "${message_generation_target}" AND TARGET "${dependency_generation_target}")
			add_dependencies(${message_generation_target}
				"${dependency_generation_target}"
			)
		endif()
	endforeach()
endfunction()
