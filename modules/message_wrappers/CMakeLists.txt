add_library(dls_message_wrappers 
	SHARED
		src/attitude.cpp
		src/base_state.cpp
		src/blind_state.cpp
		src/control_signal.cpp
		src/controller_command.cpp
		src/desired_torques.cpp
		src/imu.cpp
		src/debug/leg_odometry_debug.cpp
		src/legs_pose.cpp
		src/pose.cpp
		src/screw.cpp
		src/sensor_fusion.cpp		
		src/slip_flag.cpp
		src/t265_odometry.cpp
		src/debug/trunk_controller_debug.cpp
		src/trajectory_generator.cpp
		src/vicon.cpp
		src/stance_status.cpp
		src/mpc_generator_output.cpp
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