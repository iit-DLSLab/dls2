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
# Libraries
# ==============================================================================
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

	# Components
	application_framework/components/app_layer_component.cpp
)
target_link_libraries(
	framework
	pthread
	dl
)

# ============================ Controller Modules ==============================
add_library(
	controlinterface
	SHARED
	controller/controller.cpp
)

add_library(
	gaitgeneratorinterface
	SHARED
	gait_generator/gait_generator.cpp
)

# ================================= Geometry ===================================
add_library(
	geometry
	SHARED
	geometry/pose.cpp
)

# ==============================================================================
# Main Executable
# ==============================================================================
add_executable(
	${EXECUTABLE_NAME}

	main.cpp

	# Sensor Classes
	sensors/sensor_base.cpp
)

target_link_libraries(
	${EXECUTABLE_NAME}

	# note that the order of these is important
	framework
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
target_link_libraries(dummy_controller geometry)

add_library(
	dummy_gait_generator
	SHARED
	gait_generator/dummy_gait_generator.cpp
)

# ==============================================================================
# Convencience Targets
# ==============================================================================
# TODO remove dummy_controller from here
add_custom_target(
	run
	COMMAND ./${EXECUTABLE_NAME}
	DEPENDS ${EXECUTABLE_NAME} dummy_controller
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
