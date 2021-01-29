message(WARNING "This module is not modular - it is still taking a file from elsewhere")
add_library(dls_controller
	SHARED
	src/controller.cpp
	src/control_signal.cpp

	# # TODO this should be moved to the messaging library
	# ${PROJECT_SOURCE_DIR}/src/fastrtps_wrappers/blind_state.cpp
)
target_include_directories(dls_controller
	PUBLIC
		include
)
target_link_libraries(dls_controller
	dls_framework_components
	dls_gait_signal
)
dls_install(dls_controller)

# add_library(dls_dummy_controller SHARED
# 	src/dummy_controller.cpp
# )
# target_link_libraries(dls_dummy_controller_interface dls_geometry)
# target_link_libraries(dls_dummy_controller
# 	doglib
# )

# set(DLS_INSTALL_DUMMY_CONTROLLER OFF
# 	CACHE BOOL "whether to install the dummy controller"
# )

# if(DLS_INSTALL_DUMMY_CONTROLLER)
# 	install(TARGETS
# 		dls_dummy_controller
# 		LIBRARY DESTINATION ${DLS_INSTALL_CONTROLLER_DIR}
# 	)
# endif()
