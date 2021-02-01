add_library(dls_message_wrappers
	src/screw.cpp
	src/joint_state.cpp
	src/blind_state.cpp
)

target_include_directories(dls_message_wrappers
	PUBLIC
		include
)

target_link_libraries(dls_message_wrappers
	PUBLIC
		dls_robot
		dls_geometry
)

dls_install(dls_message_wrappers)
