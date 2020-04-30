add_library(dls_controller_interface
	SHARED
	controller.cpp
	control_signal.cpp

	# TODO this should be moved to the messaging library
	${PROJECT_SOURCE_DIR}/src/fastrtps_wrappers/blind_state.cpp
)
dls_install(dls_controller_interface)
target_link_libraries(dls_controller_interface dls_geometry)

add_library(dls_dummy_controller SHARED
	dummy_controller.cpp
)
target_link_libraries(dls_dummy_controller
	doglib
)
# add_dependencies(dls_dummy_controller messagelib)
# dls_install_debug(dls_dummy_controller)
set(DLS_INSTALL_DUMMY_CONTROLLER OFF
	CACHE BOOL "whether to install the dummy controller"
)
if(DLS_INSTALL_DUMMY_CONTROLLER)
	install(TARGETS dls_dummy_controller
		LIBRARY DESTINATION ${DLS_INSTALL_CONTROLLER_DIR}
		# COMPONENT "PLUGIN-${PROJECT_NAME}-dls_dummy_controller"
	)
endif()
