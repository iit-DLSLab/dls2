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
	-std=c++17

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

# ==============================================================================
# Libraries
# ==============================================================================

# =================================== Logging ==================================
add_library(
	dls_logging
	SHARED
	${CMAKE_SOURCE_DIR}/src/util/log/log.cpp
)

target_link_libraries(
	dls_logging

	fastcdr
	fastrtps
)
list(APPEND PROJECT_TARGETS dls_logging)
# ================================== messages ==================================
add_library(
	dls_messaging
	SHARED
	${MESSAGE_SOURCES}
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
)
target_link_libraries(
	dls_framework
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
	controller/control_signal.cpp
)
list(APPEND PROJECT_TARGETS dls_controlinterface)
target_link_libraries(dls_controlinterface dls_geometry)

add_library(
	dls_gaitgeneratorinterface
	SHARED
	gait_generator/gait_generator.cpp
	gait_generator/gait_signal.cpp
)
list(APPEND PROJECT_TARGETS dls_gaitgeneratorinterface)
target_link_libraries(dls_gaitgeneratorinterface dls_geometry)

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
)
list(APPEND PROJECT_TARGETS dls_controller_process)

add_executable(
	dls_gait_generator_process

	${CMAKE_SOURCE_DIR}/src/child_process/gait_generator/gait_generator_process.cpp
)
target_link_libraries(
	dls_gait_generator_process
	fastcdr
	fastrtps
	dl
	dls_framework
	dls_controlinterface
	dls_gaitgeneratorinterface
	dls_estimatorinterface
)
list(APPEND PROJECT_TARGETS dls_gait_generator_process)

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

add_dependencies(${EXECUTABLE_NAME} messagelib dls_controller_process dls_gait_generator_process)

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
	dls_dummy_gait_generator
	SHARED
	gait_generator/dummy_gait_generator.cpp
)
add_dependencies(dls_dummy_gait_generator messagelib)
list(APPEND PROJECT_TARGETS dls_dummy_gait_generator)

add_library(
	dls_dummy_estimator
	SHARED
	estimator/dummy_estimator.cpp
)
add_dependencies(dls_dummy_estimator messagelib)
list(APPEND PROJECT_TARGETS dls_dummy_estimator)
# target_link_libraries(dummy_gait_generator dls_geometry)

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
