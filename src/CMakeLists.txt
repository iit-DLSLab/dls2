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
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
set(EXECUTABLE_NAME dls2)

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
)

# ==============================================================================
# Forward declaration of main executable
# ==============================================================================
add_executable(${EXECUTABLE_NAME} main.cpp)

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

# ==============================================================================
# Libraries
# ==============================================================================
# ================================== messages ==================================
add_library(
	messaging
	SHARED
	${MESSAGE_SOURCES}
)
target_compile_options(
	messaging
	PUBLIC
	-w # Suppress all warnings, this library's code is auto-generated
)
add_custom_target(messagelib DEPENDS ${MESSAGE_SOURCES})

# ==================================== Debug ===================================
add_library(
	debuglib
	SHARED
	util/debug/debug.cpp
)
# ================================= Geometry ===================================
add_library(
	geometry
	SHARED
	geometry/pose.cpp
)
# ============================ Application Layers ==============================
add_library(
	framework

	SHARED
	# Framework Classes
	application_framework/hyq_app.cpp
	application_framework/app_layer.cpp
	application_framework/hardware_layer.cpp
	application_framework/hyq_app_libraries.cpp
	application_framework/control_layer.cpp
	application_framework/estimation_layer.cpp

	# Components
	application_framework/components/app_layer_component.cpp
	application_framework/components/periodic_app_layer_component.cpp
)
target_link_libraries(
	framework
	messaging
	pthread
	dl
	debuglib
)

# ============================ Controller Modules ==============================
add_library(
	estimatorinterface
	SHARED
	estimator/estimator.cpp
)
add_library(
	controlinterface
	SHARED
	controller/controller.cpp
)
target_link_libraries(controlinterface geometry)

add_library(
	gaitgeneratorinterface
	SHARED
	gait_generator/gait_generator.cpp
	gait_generator/gait_signal.cpp
)
target_link_libraries(gaitgeneratorinterface geometry)


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

add_dependencies(${EXECUTABLE_NAME} messagelib)

target_link_libraries(
	${EXECUTABLE_NAME}

	# fastrtps
	fastcdr
	fastrtps

	# note that the order of these is important
	framework
	estimatorinterface
	controlinterface
	gaitgeneratorinterface
)

# Count TODOs after build
add_custom_command(
	TARGET ${EXECUTABLE_NAME}
	POST_BUILD
	COMMAND ${CMAKE_SOURCE_DIR}/build_tools/list_todos
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Temporary test class for controller plugin interface
add_library(
	dummy_controller
	SHARED
	controller/dummy_controller.cpp
)
# target_link_libraries(dummy_controller geometry)

add_library(
	dummy_gait_generator
	SHARED
	gait_generator/dummy_gait_generator.cpp
)
add_library(
	dummy_estimator
	SHARED
	estimator/dummy_estimator.cpp
)
add_dependencies(dummy_estimator messagelib)
# target_link_libraries(dummy_gait_generator geometry)

# ==============================================================================
# Convencience Targets
# ==============================================================================
# TODO remove dummy_controller from here
add_custom_target(
	run
	COMMAND ./${EXECUTABLE_NAME}
	DEPENDS ${EXECUTABLE_NAME} dummy_controller dummy_gait_generator dummy_estimator
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
