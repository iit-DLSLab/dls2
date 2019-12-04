# To ease external collaboration, the framework is built as a series of dynamic
# libraries. These libraries are linked statically into the executable. External
# plugins are compiled as shared objects and loaded at runtime. In doing so, the
# external plugins do not need access to the source code implementing base
# classes, but only headers of the relevant (potentially not abstract) classes.

# Note that the external collaborators do not need the shared object libraries,
# they only need the final executable

# ==============================================================================
# Output Directories and CMake Variables
# ==============================================================================
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${BUILD_PREFIX}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${BUILD_PREFIX}")
set(EXECUTABLE_NAME dynamic_legged_systems_framework)

set(PROJECT_TARGETS "")
# ==============================================================================
# Global Compilation Options
# ==============================================================================
add_compile_options(
	# Standards
	-std=c++14

	# Warnings
	#	-Wfatal-errors
	-Wall
	-Wextra
	-pedantic
	-Weffc++

	# Elevate warnings to errors
	-Werror

	# TODO optimistation options

	# Custom defintions
	-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}
	# -DSHOW_TODOS_AT_COMPILE_TIME
	-DPROJECT_MAJOR=${MAJOR}
	-DPROJECT_MINOR=${MINOR}
	-DPROJECT_PATCH=${PATCH}
	-include ${CMAKE_SOURCE_DIR}/include/util/debug/debug.hpp
	# -include ${CMAKE_SOURCE_DIR}/build_tools/include/todo.h
)

# ==============================================================================
# Forward declaration of main executable
# ==============================================================================
add_executable(${EXECUTABLE_NAME} main.cpp)
list(APPEND PROJECT_TARGETS ${EXECUTABLE_NAME})

# ==============================================================================
# Header Export
# ==============================================================================
set(EXPORT_HEADERS "")
macro(export_header header)
	list(APPEND EXPORT_HEADERS header)
endmacro()

# ==============================================================================
# Message Generation
# ==============================================================================
set(MESSAGE_SOURCES "")
macro(add_message msg)
	add_custom_command(
		OUTPUT
			${CMAKE_SOURCE_DIR}/gen/include/msg/${msg}.cxx
			${CMAKE_SOURCE_DIR}/gen/include/msg/${msg}.h
			${CMAKE_SOURCE_DIR}/gen/include/msg/${msg}PubSubTypes.cxx
			${CMAKE_SOURCE_DIR}/gen/include/msg/${msg}PubSubTypes.h
		COMMAND
			[ -d ${CMAKE_SOURCE_DIR}/gen/include/msg ] || mkdir --parents ${CMAKE_SOURCE_DIR}/gen/include/msg
		COMMAND
			fastrtpsgen -replace ${CMAKE_SOURCE_DIR}/msg/${msg}.idl -d ${CMAKE_SOURCE_DIR}/gen/include/msg
		COMMENT
			"Generating message files for ${msg}.idl"
		DEPENDS
			${CMAKE_SOURCE_DIR}/msg/${msg}.idl
	)
	list(APPEND MESSAGE_SOURCES ${CMAKE_SOURCE_DIR}/gen/include/msg/${msg}.cxx)
	list(APPEND MESSAGE_SOURCES ${CMAKE_SOURCE_DIR}/gen/include/msg/${msg}PubSubTypes.cxx)
endmacro()

add_message(hello_world)
add_message(pose)
add_message(gait_signal)
add_message(control_signal)
add_message(desired_torques)
add_message(stringmsg)
add_message(joint_state)
add_message(imu)
add_message(vec3)
add_message(screw)
add_message(blind_state)
add_message(console_command)
add_message(header)
add_message(time)
add_message(bool)

# ==============================================================================
# Libraries
# ==============================================================================

# =================================== Logging ==================================
add_library(
	dls_logging
	SHARED
	${CMAKE_SOURCE_DIR}/src/util/log/log.cpp
)

target_link_libraries(dls_logging
	PRIVATE
		fastcdr
		fastrtps
)
list(APPEND PROJECT_TARGETS dls_logging)

# ==================================== util ====================================
# add_library(dls_string_util
# 	SHARED
# 	${CMAKE_SOURCE_DIR}/src/util/string/string.cpp
# )
# list(APPEND PROJECT_TARGETS string_util)
# ================================== messages ==================================
add_library(dls_messaging
	SHARED

	# fastrtps
	${MESSAGE_SOURCES}

	# generic wrappers
	${CMAKE_SOURCE_DIR}/src/msg_wrappers/screw.cpp
	${CMAKE_SOURCE_DIR}/src/msg_wrappers/joint_state.cpp
	${CMAKE_SOURCE_DIR}/src/controller/control_signal.cpp
)
target_compile_options(
	dls_messaging
	PRIVATE
	-w # Suppress all warnings, this library's code is auto-generated
)
list(APPEND PROJECT_TARGETS dls_messaging)
add_custom_target(messagelib DEPENDS ${MESSAGE_SOURCES})

# ==================================== Debug ===================================
add_library(
	dls_debug
	SHARED
	util/debug/debug.cpp
)
list(APPEND PROJECT_TARGETS dls_debug)

# ================================= Geometry ===================================
add_library(
	dls_geometry
	SHARED
	geometry/pose.cpp
)
add_dependencies(dls_geometry dls_messaging)
list(APPEND PROJECT_TARGETS dls_geometry)
# ============================ Application Layers ==============================
add_library(
	dls_framework

	SHARED
	# Framework Classes
	application_framework/hyq_app.cpp
	application_framework/app_layer.cpp
	application_framework/hardware_layer.cpp
	application_framework/hyq_app_libraries.cpp
	application_framework/control_layer.cpp
	application_framework/console_layer.cpp
	application_framework/estimation_layer.cpp
	application_framework/log_layer.cpp

	# Components
	application_framework/components/app_layer_component.cpp
	application_framework/components/periodic_app_layer_component.cpp

	# TODO remove this and instead link to the library
	util/string/string.cpp
)
target_link_libraries(dls_framework
	PRIVATE
		readline # for console_layer
		fastcdr
		fastrtps
		dls_time

	PUBLIC # TODO fix this
	#dls_string_util TODO link to this library and remove the source above
		dls_logging
		dls_messaging
		pthread
		dl
		dls_debug
)
list(APPEND PROJECT_TARGETS dls_framework)
# ============================ Controller Modules ==============================
add_library(
	dls_estimatorinterface
	SHARED
	estimator/estimator.cpp
)
list(APPEND PROJECT_TARGETS dls_estimatorinterface)
add_library(
	dls_controlinterface
	SHARED
	controller/controller.cpp

	# TODO this should be moved to the messaging library
	controller/control_signal.cpp
	fastrtps_wrappers/blind_state.cpp
)
list(APPEND PROJECT_TARGETS dls_controlinterface)
target_link_libraries(dls_controlinterface dls_geometry)

add_library(
	dls_gaitgeneratorinterface
	SHARED
	gait_generator/gait_generator.cpp
	# TODO this should be moved to the messaging library
	gait_generator/gait_signal.cpp
)
list(APPEND PROJECT_TARGETS dls_gaitgeneratorinterface)
target_link_libraries(dls_gaitgeneratorinterface dls_geometry)

# ==================================== Time ====================================
add_library(dls_time
	SHARED
	${CMAKE_SOURCE_DIR}/src/util/time/time.cpp
)
list(APPEND PROJECT_TARGETS dls_time)

# ==============================================================================
# Child Processes
# ==============================================================================
add_executable(
	dls_controller_process

	${CMAKE_SOURCE_DIR}/src/child_process/controller/controller_process.cpp
)
target_link_libraries(
	dls_controller_process
	fastcdr
	fastrtps
	dl
	dls_framework
	dls_controlinterface
	dls_gaitgeneratorinterface
	dls_estimatorinterface

	#PRIVATE
	dls_time
)
list(APPEND PROJECT_TARGETS dls_controller_process)

add_executable(
	dls_gait_generator_process

	${CMAKE_SOURCE_DIR}/src/child_process/gait_generator/gait_generator_process.cpp
)
target_link_libraries(dls_gait_generator_process
	dls_framework
	fastcdr
	fastrtps
	dl
	dls_controlinterface
	dls_gaitgeneratorinterface
	dls_estimatorinterface

	#PRIVATE
	dls_time
)
list(APPEND PROJECT_TARGETS dls_gait_generator_process)
# ==============================================================================
# Roscontrol Tranlsation Block
# ==============================================================================
set(CATKIN_WORKSPACE ${CMAKE_SOURCE_DIR}/catkin_ws)
add_custom_target(dls2_to_roscontrol
	COMMAND catkin_make
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/catkin_ws
	COMMENT "Building dls2_to_roscontrol subproject"
)

set_property(
	DIRECTORY
		${CMAKE_SOURCE_DIR}
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
		${CATKIN_WORKSPACE}/build
		${CATKIN_WORKSPACE}/devel
)
add_dependencies(dls2_to_roscontrol dls_framework dls_messaging)

# ==============================================================================
# Main Executable
# ==============================================================================
target_sources(
	${EXECUTABLE_NAME}

	PUBLIC

	# current version of cmake does not allow to specify target with no sources
	# initialy, hence this line added earlier and not here
	# main.cpp

	# Sensor Classes
	sensors/sensor_base.cpp
)

add_dependencies(${EXECUTABLE_NAME}
	messagelib
	dls_controller_process
	dls_gait_generator_process
	dls2_to_roscontrol
	# dls_pid_controller
	# dls_prep_gait_generator
)

target_link_libraries(
	${EXECUTABLE_NAME}

	# fastrtps
	fastcdr
	fastrtps

	# note that the order of these is important
	dls_framework
	dls_estimatorinterface
	dls_controlinterface
	dls_gaitgeneratorinterface
)

# Count TODOs after build
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_custom_command(
		TARGET ${EXECUTABLE_NAME}
		POST_BUILD
		COMMAND ${CMAKE_SOURCE_DIR}/build_tools/list_todos
		COMMAND [ -f ${CMAKE_SOURCE_DIR}/bin/debug/launch_simulation_backend.sh ] || ln -s ${CMAKE_SOURCE_DIR}/scripts/launch_simulation_backend.sh ${CMAKE_SOURCE_DIR}/bin/debug/launch_simulation_backend.sh
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
endif()

# Temporary test class for controller plugin interface
add_library(
	dls_dummy_controller
	SHARED
	controller/dummy_controller.cpp
)
add_dependencies(dls_dummy_controller messagelib)
list(APPEND PROJECT_TARGETS dls_dummy_controller)
# target_link_libraries(dummy_controller dls_geometry)

add_library(
	dls_pid_controller
	SHARED
	controller/pid_controller.cpp
)
target_link_libraries(dls_pid_controller dls_logging)
add_dependencies(dls_pid_controller messagelib)
list(APPEND PROJECT_TARGETS dls_pid_controller)

add_library(
	dls_dummy_gait_generator
	SHARED
	gait_generator/dummy_gait_generator.cpp
)
add_dependencies(dls_dummy_gait_generator messagelib)
list(APPEND PROJECT_TARGETS dls_dummy_gait_generator)

add_library(
	dls_prep_gait_generator
	SHARED
	gait_generator/prep_gait_generator.cpp
)
add_dependencies(dls_prep_gait_generator messagelib)
list(APPEND PROJECT_TARGETS dls_prep_gait_generator)

add_library(
	dls_dummy_estimator
	SHARED
	estimator/dummy_estimator.cpp
)
add_dependencies(dls_dummy_estimator messagelib)
list(APPEND PROJECT_TARGETS dls_dummy_estimator)
# target_link_libraries(dummy_gait_generator dls_geometry)

# ==============================================================================
# Tools
# ==============================================================================
add_executable(topic_info
	${CMAKE_SOURCE_DIR}/src/util/topic_info/topic_info.cpp
	${CMAKE_SOURCE_DIR}/src/util/topic_info/topic_info_main.cpp

	# TODO this should be put into the dls_messaging library and just linked
	# into this executable. It should NOT be recompiled here, it should only be
	# compiled in the library
	${CMAKE_SOURCE_DIR}/src/controller/control_signal.cpp
)
target_link_libraries(topic_info
	fastcdr
	fastrtps
	dls_messaging
	dls_debug

	# TODO this should be removed when the gait signal and contro lsignal
	# wrappers are moved to the messaging library
	# dls_gaitgeneratorinterface
	# dls_controlinterface
	# dls_estimatorinterface
	# dls_framework
)

# ==============================================================================
# Convencience Targets
# ==============================================================================
# TODO remove dummy_controller from here
add_custom_target(
	run
	COMMAND ./${EXECUTABLE_NAME}
	DEPENDS ${EXECUTABLE_NAME} dls_dummy_controller dls_dummy_gait_generator dls_dummy_estimator
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	install(TARGETS ${PROJECT_TARGETS}
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
		PUBLIC_HEADER DESTINATION dls/include
	)
	install(
		DIRECTORY
			${CMAKE_SOURCE_DIR}/include/
			${CMAKE_SOURCE_DIR}/build_tools/include/
			${CMAKE_SOURCE_DIR}/gen/include/
		DESTINATION
			include/dls
		FILES_MATCHING
			PATTERN "*.h"
			PATTERN "*.hpp"
			PATTERN "*.tpp"
	)
endif()
