include(dls_include_paths)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
	message(STATUS "from src:dir=${dir}")
endforeach()
message(STATUS "location=${dls_${PROJECT_NAME}_msg_includes}")

# ==============================================================================
# Global Compilation Options
# ==============================================================================
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
dls_install(dynamic_legged_systems_framework)

# ==============================================================================
# Debugger
# ==============================================================================
add_custom_target(debug
	COMMAND           gdb ${EXECUTABLE_OUTPUT_PATH}/dynamic_legged_systems_framework
	DEPENDS           dynamic_legged_systems_framework
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
	COMMENT           "Launching debugger"
)

# ==============================================================================
# Header Export
# ==============================================================================
set(EXPORT_HEADERS "")
macro(export_header header)
	list(APPEND EXPORT_HEADERS header)
endmacro()

# ==============================================================================
# Libraries
# ==============================================================================
# ================================== messages ==================================
add_library(dls_messaging
	SHARED

	# fastrtps
	# ${MESSAGE_SOURCES}

	# generic wrappers
	${PROJECT_SOURCE_DIR}/src/msg_wrappers/screw.cpp
	${PROJECT_SOURCE_DIR}/src/msg_wrappers/joint_state.cpp
	${PROJECT_SOURCE_DIR}/src/controller/control_signal.cpp
)
target_compile_options(dls_messaging
	PRIVATE
	-w # Suppress all warnings, this library's code is auto-generated
)
target_link_libraries(dls_messaging
	PUBLIC
	dls_${PROJECT_NAME}_messaging
)
add_dependencies(dls_messaging dls_dls2_messaging)

dls_install(dls_messaging)

# ============================ Controller Modules ==============================
add_library(dls_reflex
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/reflex/reflex
)

# ==================================== Time ====================================
# ==============================================================================
# Roscontrol Tranlsation Block
# ==============================================================================
# START
set(CATKIN_WORKSPACE ${PROJECT_SOURCE_DIR}/catkin_ws)
add_custom_target(dls2_to_roscontrol
	COMMAND           catkin_make -j1 install && cp -r ${CATKIN_WORKSPACE}/install ${CATKIN_WORKSPACE}/dls2_ws
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/catkin_ws
	COMMENT           "Building dls2_to_roscontrol subproject"
)
install(
	DIRECTORY              ${CATKIN_WORKSPACE}/dls2_ws/install
	DESTINATION            ${DLS_CATKIN_WORKSPACE_DIR}
	COMPONENT              dls_ws
	USE_SOURCE_PERMISSIONS
)

set(CPACK_DEBIAN_DLS_WS_FILE_NAME
;"dls2-workspace-${PROJECT_VERSION}.deb"
	PARENT_SCOPE
)
set(CPACK_DEBIAN_DLS_WS_PACKAGE_NAME
	"dls2-workspace"
	PARENT_SCOPE
)
# END

set_property(
	DIRECTORY
		${PROJECT_SOURCE_DIR}
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
		${CATKIN_WORKSPACE}/build
		${CATKIN_WORKSPACE}/devel
		${CATKIN_WORKSPACE}/install
		${CATKIN_WORKSPACE}/dls2_ws
)
add_dependencies(dls2_to_roscontrol dls_framework dls_messaging)

# ==============================================================================
# Main Executable
# ==============================================================================

add_dependencies(dynamic_legged_systems_framework
	# messagelib
	child_process_launcher
	dls2_to_roscontrol
)

target_link_libraries(
	dynamic_legged_systems_framework
	dls_${PROJECT_NAME}_messaging

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
get_property(LAUNCH_FRAMEOWRK_FROM_SRC_TREE
	TARGET   launch_framework_from_src_tree
	PROPERTY INTERFACE_SOURCES
)
string(REPLACE " " ";" MAKE_RUN_ARGS ${DLS_BUILD_MAKE_RUN_FLAGS})
add_custom_target(
	run
	COMMAND bash ${LAUNCH_FRAMEOWRK_FROM_SRC_TREE} ${MAKE_RUN_ARGS}
	DEPENDS
		dynamic_legged_systems_framework
		dls_dummy_controller
		dls_dummy_gait_generator
		dls_dummy_estimator
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
