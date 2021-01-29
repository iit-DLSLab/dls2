add_library(dls_messaging
	SHARED

	# generic wrappers
	src/screw.cpp
	src/joint_state.cpp
	src/blind_state.cpp
	# src/control_signal.cpp
)
target_include_directories(dls_messaging PUBLIC include)
target_compile_options(dls_messaging
	PRIVATE
	-w
)
target_link_libraries(dls_messaging
	PUBLIC
	dls_robot
	dls_geometry
)
target_include_directories(dls_messaging
	PUBLIC include
)
add_dependencies(dls_messaging dls_dls2_messaging)

dls_install(dls_messaging)

