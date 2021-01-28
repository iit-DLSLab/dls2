message(WARNING "This module is not modular -- it still takes a file from elsewhere")
add_library(dls_gait_generator
	SHARED
	src/gait_generator.cpp
	# TODO this should be moved to the messaging library
	src/gait_signal.cpp

	# TODO this should be moved to the messaging library
	${PROJECT_SOURCE_DIR}/src/fastrtps_wrappers/blind_state.cpp
)
target_include_directories(dls_gait_generator PUBLIC include)
dls_install(dls_gait_generator)

target_link_libraries(dls_gait_generator
	dls_geometry
	doglib
	dls_framework
	dls_logging
)
add_library(dls_dummy_gait_generator
	SHARED src/dummy_gait_generator.cpp
)
target_include_directories(dls_dummy_gait_generator PUBLIC include)
add_dependencies(dls_gait_generator dls_dls2_messaging)
# add_dependencies(dls_dummy_gait_generator messagelib)

# dls_install_debug(dls_dummy_gait_generator)
set(DLS_INSTALL_DUMMY_GAIT_GENERATOR OFF
	CACHE BOOL "whether to install the dummy gait generator"
)
if(DLS_INSTALL_DUMMY_GAIT_GENERATOR)
	install(TARGETS dls_dummy_gait_generator
		LIBRARY DESTINATION ${DLS_INSTALL_GAIT_GENERATOR_DIR}
		# COMPONENT "PLUGIN-${PROJECT_NAME}-dls_dummy_gait_generator"
	)
endif()
target_link_libraries(dls_dummy_gait_generator
	doglib
)
