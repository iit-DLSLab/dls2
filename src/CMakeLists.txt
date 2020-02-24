include(dls_include_paths)
# ==============================================================================
# Output Directories and CMake Variables
# ==============================================================================
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

# ==============================================================================
# Global Compilation Options
# ==============================================================================
add_compile_options(
	# Standards
	-std=c++14

	# Warnings
	-Wall
	-Wextra
	-pedantic
	-Weffc++

	# Elevate warnings to errors
	-Werror

	# TODO optimistation options

	# Custom defintions
	-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}
	-include ${PROJECT_SOURCE_DIR}/include/dls2/util/debug/debug.hpp
)

add_subdirectory(application_framework)
add_subdirectory(child_process)
add_subdirectory(command)
add_subdirectory(controller)
add_subdirectory(estimator)
add_subdirectory(gait_generator)
add_subdirectory(geometry)
add_subdirectory(util)

# ==============================================================================
# Forward declaration of main executable
# ==============================================================================
configure_file(
	${PROJECT_SOURCE_DIR}/src/main.cpp.in
	${CMAKE_BINARY_DIR}/src/main.cpp
)
# add_executable(dynamic_legged_systems_framework main.cpp)
add_executable(dynamic_legged_systems_framework
	${CMAKE_BINARY_DIR}/src/main.cpp
)
# list(APPEND PROJECT_TARGETS dynamic_legged_systems_framework)
dls_install(dynamic_legged_systems_framework)

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
			${PROJECT_SOURCE_DIR}/gen/include/dls2/msg/${msg}.cxx
			${PROJECT_SOURCE_DIR}/gen/include/dls2/msg/${msg}.h
			${PROJECT_SOURCE_DIR}/gen/include/dls2/msg/${msg}PubSubTypes.cxx
			${PROJECT_SOURCE_DIR}/gen/include/dls2/msg/${msg}PubSubTypes.h
		COMMAND
			[ -d ${PROJECT_SOURCE_DIR}/gen/include/dls2/msg ] || mkdir --parents ${PROJECT_SOURCE_DIR}/gen/include/dls2/msg
		COMMAND
			fastrtpsgen -replace ${PROJECT_SOURCE_DIR}/msg/${msg}.idl -d ${PROJECT_SOURCE_DIR}/gen/include/dls2/msg
		COMMENT
			"Generating message files for ${msg}.idl"
		DEPENDS
			${PROJECT_SOURCE_DIR}/msg/${msg}.idl
	)
	list(APPEND MESSAGE_SOURCES ${PROJECT_SOURCE_DIR}/gen/include/dls2/msg/${msg}.cxx)
	list(APPEND MESSAGE_SOURCES ${PROJECT_SOURCE_DIR}/gen/include/dls2/msg/${msg}PubSubTypes.cxx)
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
add_message(command_register)
add_message(el3104)
add_message(hpu)
add_message(pcb)
add_message(psb)
add_message(smart_actuator)
add_message(battery)
add_message(heatsink)
add_message(leg_raw)
add_message(hyqreal_raw)

# ==============================================================================
# Libraries
# ==============================================================================

# ================================== messages ==================================
add_library(dls_messaging
	SHARED

	# fastrtps
	${MESSAGE_SOURCES}

	# generic wrappers
	${PROJECT_SOURCE_DIR}/src/msg_wrappers/screw.cpp
	${PROJECT_SOURCE_DIR}/src/msg_wrappers/joint_state.cpp
	${PROJECT_SOURCE_DIR}/src/controller/control_signal.cpp
)
target_compile_options(
	dls_messaging
	PRIVATE
	-w # Suppress all warnings, this library's code is auto-generated
)
# list(APPEND PROJECT_TARGETS dls_messaging)
dls_install(dls_messaging)
add_custom_target(messagelib DEPENDS ${MESSAGE_SOURCES})

# ============================ Controller Modules ==============================
add_library(dls_reflex
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/reflex/reflex
)

# ==================================== Time ====================================
# ==============================================================================
# Roscontrol Tranlsation Block
# ==============================================================================
set(CATKIN_WORKSPACE ${PROJECT_SOURCE_DIR}/catkin_ws)
add_custom_target(dls2_to_roscontrol
	COMMAND catkin_make
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/catkin_ws
	COMMENT "Building dls2_to_roscontrol subproject"
)
add_custom_command(
	TARGET dls2_to_roscontrol
	POST_BUILD
	COMMAND cp ${CATKIN_WORKSPACE}/devel/lib/dls_hal/xenomotor ${EXECUTABLE_OUTPUT_PATH}
	COMMAND cp ${CATKIN_WORKSPACE}/devel/lib/dls_hal/xenorostask ${EXECUTABLE_OUTPUT_PATH}
)

set_property(
	DIRECTORY
		${PROJECT_SOURCE_DIR}
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
		${CATKIN_WORKSPACE}/build
		${CATKIN_WORKSPACE}/devel
)
add_dependencies(dls2_to_roscontrol dls_framework dls_messaging)

# ==============================================================================
# Main Executable
# ==============================================================================

add_dependencies(dynamic_legged_systems_framework
	messagelib
	# dls_controller_process
	# dls_gait_generator_process
	child_process_launcher
	dls2_to_roscontrol
)

target_link_libraries(
	dynamic_legged_systems_framework

	# fastrtps
	fastcdr
	fastrtps

	# note that the order of these is important
	dls_framework
	dls_estimator
	dls_controller_interface
	dls_gait_generator
)

# ==============================================================================
# Convencience Targets
# ==============================================================================
string(REPLACE " " ";" MAKE_RUN_ARGS ${DLS_BUILD_MAKE_RUN_FLAGS})
add_custom_target(
	run
	COMMAND ./dynamic_legged_systems_framework ${MAKE_RUN_ARGS}
	DEPENDS
		dynamic_legged_systems_framework
		dls_dummy_controller
		dls_dummy_gait_generator
		dls_dummy_estimator
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
