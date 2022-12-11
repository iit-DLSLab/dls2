add_library(dls_message_wrappers 
	SHARED
		src/screw.cpp
		src/blind_state.cpp
		src/pose.cpp
		src/gait_signal.cpp
		src/control_signal.cpp
		src/controller_command.cpp
		src/foot_state.cpp
		src/traj_gen_signal.cpp
)

set_property(TARGET dls_message_wrappers PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_message_wrappers
	PUBLIC
		include
)

target_link_libraries(dls_message_wrappers
	PUBLIC
		robotlib	
		dls_messages
		dls_math
		dls_com
		dls_topics
		dls_domains			
)

dls_install(dls_message_wrappers)