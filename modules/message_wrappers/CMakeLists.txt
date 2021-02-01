add_library(dls_messaging SHARED
	src/screw.cpp
	src/joint_state.cpp
	src/blind_state.cpp
)

target_include_directories(dls_messaging
	PUBLIC
		include
)

target_link_libraries(dls_messaging
	PUBLIC
		dls_robot
		dls_geometry
)

add_dependencies(dls_messaging
	dls_dls2_messaging
)

dls_install(dls_messaging)
