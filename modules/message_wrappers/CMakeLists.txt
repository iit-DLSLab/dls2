add_library(dls_message_wrappers 
	SHARED
		src/screw.cpp
		src/blind_state.cpp
		src/point.cpp
		src/pose.cpp
		src/gait_signal.cpp
		src/control_signal.cpp
		src/controller_command.cpp
		src/foot_state.cpp
		src/traj_gen_signal.cpp
		src/odometry.cpp
		src/legs_pose.cpp
		src/attitude.cpp
		src/translation.cpp		
		src/vicon.cpp
		src/imu.cpp
		src/desired_torques.cpp
		src/stance_legs.cpp
		src/base_state.cpp
		src/debug/leg_odometry_debug.cpp
		src/debug/trunk_controller_debug.cpp
)
set_property(TARGET dls_message_wrappers PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_message_wrappers
	PUBLIC
		include
)

target_link_libraries(dls_message_wrappers
	PRIVATE
		robotlib	
		dls_messages
		dls_math
		dls_com
		dls_topics
		dls_domains			
)

dls_install(dls_message_wrappers)