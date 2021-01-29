configure_file(
	src/main.cpp.in
	${CMAKE_CURRENT_BINARY_DIR}/src/main.cpp
)

add_executable(dynamic_legged_systems_framework
	${CMAKE_CURRENT_BINARY_DIR}/src/main.cpp
)
dls_install(dynamic_legged_systems_framework)

add_dependencies(dynamic_legged_systems_framework
	# messagelib
	child_process_launcher
	# dls2_to_roscontrol
)

target_link_libraries(
	dynamic_legged_systems_framework

	# note that the order of these is important
	dls_framework
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
