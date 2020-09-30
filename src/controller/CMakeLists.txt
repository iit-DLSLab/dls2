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

add_dependencies(dls_controller_interface dls_dls2_messaging)
# add_dependencies(dls_dummy_controller messagelib)
# dls_install_debug(dls_dummy_controller)
set(DLS_INSTALL_DUMMY_CONTROLLER OFF
	CACHE BOOL "whether to install the dummy controller"
)



add_library(dls_dummy_controller2 SHARED
	dummy_controller2.cpp
)
target_link_libraries(dls_dummy_controller2
	doglib
)
add_library(dls_dummy_controller3 SHARED
	dummy_controller3.cpp
)
target_link_libraries(dls_dummy_controller3
	doglib
)
add_library(dls_dummy_controller4 SHARED
	dummy_controller4.cpp
)
target_link_libraries(dls_dummy_controller4
	doglib
)
add_library(dls_dummy_controller5 SHARED
	dummy_controller5.cpp
)
target_link_libraries(dls_dummy_controller5
	doglib
)
add_library(dls_dummy_controller6 SHARED
	dummy_controller6.cpp
)
target_link_libraries(dls_dummy_controller6
	doglib
)
add_library(dls_dummy_controller7 SHARED
	dummy_controller7.cpp
)
target_link_libraries(dls_dummy_controller7
	doglib
)
add_library(dls_dummy_controller8 SHARED
	dummy_controller8.cpp
)
target_link_libraries(dls_dummy_controller8
	doglib
)
add_library(dls_dummy_controller9 SHARED
	dummy_controller9.cpp
)
target_link_libraries(dls_dummy_controller9
	doglib
)


if(DLS_INSTALL_DUMMY_CONTROLLER)
	install(TARGETS
		dls_dummy_controller
		dls_dummy_controller2
		dls_dummy_controller3
		dls_dummy_controller4
		dls_dummy_controller5
		dls_dummy_controller6
		dls_dummy_controller7
		dls_dummy_controller8
		dls_dummy_controller9
		LIBRARY DESTINATION ${DLS_INSTALL_CONTROLLER_DIR}
		# COMPONENT "PLUGIN-${PROJECT_NAME}-dls_dummy_controller"
	)
endif()
