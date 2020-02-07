# ==============================================================================
# Controller Library
# ==============================================================================
add_library(dls_controller
	SHARED
	controller.cpp
	control_signal.cpp

	# TODO this should be moved to the messaging library
	${CMAKE_SOURCE_DIR}/src/fastrtps_wrappers/blind_state.cpp
)
dls_install(dls_controller)
target_link_libraries(dls_controller dls_geometry)


add_library(
	dls_dummy_controller
	SHARED
	dummy_controller.cpp
)
target_link_libraries(dls_dummy_controller
	doglib
)
add_dependencies(dls_dummy_controller messagelib)
dls_install_debug(dls_dummy_controller)

add_library(
	dls_pid_controller
	SHARED
	pid_controller.cpp
)
target_link_libraries(dls_pid_controller dls_logging doglib)
add_dependencies(dls_pid_controller messagelib)
dls_install(dls_pid_controller)
