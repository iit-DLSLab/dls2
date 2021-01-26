configure_file(
	${PROJECT_SOURCE_DIR}/src/main.cpp.in
	${CMAKE_BINARY_DIR}/src/main.cpp
)

# add_executable(dynamic_legged_systems_framework main.cpp)
add_executable(dynamic_legged_systems_framework
	${CMAKE_BINARY_DIR}/src/main.cpp
)
dls_install(dynamic_legged_systems_framework)

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
