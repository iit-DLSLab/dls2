
add_compile_options("-std=c++20")

configure_file(src/main.cpp.in
	${CMAKE_CURRENT_BINARY_DIR}/src/main.cpp
)

configure_file(src/run_dls2.cpp.in
	${CMAKE_CURRENT_BINARY_DIR}/src/run_dls2.cpp
)

add_executable(dynamic_legged_systems_framework
	${CMAKE_CURRENT_BINARY_DIR}/src/main.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/run_dls2.cpp
)

target_include_directories(dynamic_legged_systems_framework
	PUBLIC
		include
)

target_link_libraries(dynamic_legged_systems_framework
	PUBLIC
		dls_framework
		dls_supervisor
		dls_param_server
		# dls_web_socket_translator
		state_machine_watcher
		fastcdr
		fastdds
		yaml-cpp
)

dls_install(dynamic_legged_systems_framework)

string(REPLACE " " ";" MAKE_RUN_ARGS ${DLS_BUILD_MAKE_RUN_FLAGS})
add_custom_target(
	run
	COMMAND bash ${LAUNCH_FRAMEOWRK_FROM_SRC_TREE} ${MAKE_RUN_ARGS}
	DEPENDS
		dynamic_legged_systems_framework
		dls_dummy_controller
		dls_dummy_motion_generator
		dls_dummy_estimator
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

add_dependencies(dynamic_legged_systems_framework
	child_process_launcher
)

install(
	FILES src/default_startup.yaml
	DESTINATION ${DLS_INSTALL_STARTUP_DIR}
	COMPONENT dls_runtime
)
