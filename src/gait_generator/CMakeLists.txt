add_library(dls_gait_generator
	SHARED
	gait_generator.cpp
	# TODO this should be moved to the messaging library
	gait_signal.cpp

	# TODO this should be moved to the messaging library
	${PROJECT_SOURCE_DIR}/src/fastrtps_wrappers/blind_state.cpp
)
dls_install(dls_gait_generator)

target_link_libraries(dls_gait_generator dls_geometry doglib)
add_library(
	dls_dummy_gait_generator
	SHARED
	dummy_gait_generator.cpp
)
# add_dependencies(dls_dummy_gait_generator messagelib)

dls_install_debug(dls_dummy_gait_generator)
target_link_libraries(dls_dummy_gait_generator
	doglib
)
