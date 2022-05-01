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
		// logs
		dls::topicType warn_log_stream 			= dls::topicType("warn_log_stream", new StringMsgPubSubType());
		dls::topicType info_log_stream 			= dls::topicType("info_log_stream", new StringMsgPubSubType());
		dls::topicType error_log_stream 		= dls::topicType("error_log_stream", new StringMsgPubSubType());
		dls::topicType fatal_log_stream 		= dls::topicType("fatal_log_stream", new StringMsgPubSubType());
		dls::topicType hyqreal_raw 				= dls::topicType("hyqreal_raw", new HyQRealRawMsgPubSubType());
		dls::topicType debug_log_stream 		= dls::topicType("debug_log_stream", new StringMsgPubSubType());

		// command
		dls::topicType command_call 			= dls::topicType("command_call", new CommandCallMsgPubSubType());
		std::string console 					= "console";

		// simulation
		std::string simulation_time 			= "simulation_time";
		dls::topicType simulation_pause 		= dls::topicType("simulation_pause", new BoolMsgPubSubType());

		// development pool
		dls::topicType develop_testbench		= dls::topicType("develop_testbench", new StringMsgPubSubType());

		// control signals
		dls::topicType desired_torques 			= dls::topicType("rt/desired_torques", new DesiredTorquesMsgPubSubType());
		dls::topicType control_signal 			= dls::topicType("rt/control_signal", new  ControlSignalMsgPubSubType());
		dls::topicType gait_signal 				= dls::topicType("rt/gaitSignal", new  GaitSignalMsgPubSubType());
		dls::topicType joint_states				= dls::topicType("rt/joint_states", new JointStateMsgPubSubType());

		// commands send
		dls::topicType command_send 			= dls::topicType("command_send", new CommandSendMsgPubSubType());		
		
		
		namespace low_level_estimation
		{
			dls::topicType blind_state 			= dls::topicType("blind_state", new  BlindStateMsgPubSubType());
			dls::topicType hyq_raw 				= dls::topicType("hyq_raw", new HyQRawMsgPubSubType());
			dls::topicType imu 					= dls::topicType("imu", new ImuMsgPubSubType());
		}
	}
}

#endif /* end of include guard: TOPICS_CPP */
