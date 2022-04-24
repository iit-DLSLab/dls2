/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef TOPICS_CPP
#define TOPICS_CPP

#include "dls2/topics/topics.hpp"

namespace dls
{

	namespace topics
	{
		std::string activate_controller 			= "console_activate_controller";
		std::string activate_gait_generator 		= "console_activate_gait_generator";
		topicType command_call 						= dls::topicType("command_call", new CommandCallMsgPubSubType());
		std::string console 						= "console";
		topicType control_signal 					= dls::topicType("control_signal", new  ControlSignalMsgPubSubType());
		std::string deactivate_controller 			= "console_deactivate_controller";
		std::string deactivate_gait_generator 		= "console_deactivate_gait_generator";
		topicType debug_log_stream 					= dls::topicType("debug_log_stream", new StringMsgPubSubType());
		topicType desired_torques 					= dls::topicType("desired_torques", new DesiredTorquesMsgPubSubType());
		topicType error_log_stream 					= dls::topicType("error_log_stream", new StringMsgPubSubType());
		topicType fatal_log_stream 					= dls::topicType("fatal_log_stream", new StringMsgPubSubType());
		std::string gait_signal 					= "GaitSignal";
		std::string gait_layer  					= "GaitSignalLayer";
		topicType hyqreal_raw 						= dls::topicType("hyqreal_raw", new HyQRealRawMsgPubSubType());
		topicType info_log_stream 					= dls::topicType("info_log_stream", new StringMsgPubSubType());
		std::string joint_states					= "joint_states";
		topicType simulation_pause 					= dls::topicType("simulation_pause", new BoolMsgPubSubType());
		std::string simulation_time 				= "simulation_time";
		topicType warn_log_stream 					= dls::topicType("warn_log_stream", new StringMsgPubSubType());
		
		namespace low_level_estimation
		{
			std::string blind_state 				= "low_level_estimation/blind_state";
			std::string hyq_raw 					= "low_level_estimation/hyq_raw";
			std::string imu 						= "low_level_estimation/imu";
		}
	}
}

#endif /* end of include guard: TOPICS_CPP */
