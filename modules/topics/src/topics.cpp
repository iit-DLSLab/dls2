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

#include <dls_messages/dds/timeTypeObject.h>
#include <dls_messages/dds/headerTypeObject.h>
#include <dls_messages/dds/stringmsgTypeObject.h>
#include <dls_messages/dds/hyqreal_rawTypeObject.h>
#include <dls_messages/dds/command_callTypeObject.h>
#include <dls_messages/dds/boolTypeObject.h>
#include <dls_messages/dds/desired_torquesTypeObject.h>
#include <dls_messages/dds/control_signalTypeObject.h>
#include <dls_messages/dds/gait_signalTypeObject.h>
#include <dls_messages/dds/joint_stateTypeObject.h>
#include <dls_messages/dds/command_sendTypeObject.h>
#include <dls_messages/dds/blind_stateTypeObject.h>
#include <dls_messages/dds/hyq_rawTypeObject.h>
#include <dls_messages/dds/aliengo_rawTypeObject.h>
#include <dls_messages/dds/imuTypeObject.h>
#include <dls_messages/dds/legs_poseTypeObject.h>

namespace dls
{

	namespace topics
	{
		// logs
		dls::topicType warn_log_stream 			= dls::topicType("warn_log_stream", new StringMsgPubSubType(), &registerstringmsgTypes);
		dls::topicType info_log_stream 			= dls::topicType("info_log_stream", new StringMsgPubSubType(), &registerstringmsgTypes);
		dls::topicType error_log_stream 		= dls::topicType("error_log_stream", new StringMsgPubSubType(), &registerstringmsgTypes);
		dls::topicType fatal_log_stream 		= dls::topicType("fatal_log_stream", new StringMsgPubSubType(), &registerstringmsgTypes);
		dls::topicType debug_log_stream 		= dls::topicType("debug_log_stream", new StringMsgPubSubType(), &registerstringmsgTypes);

		// command
		dls::topicType command_call 			= dls::topicType("command_call", new CommandCallMsgPubSubType(), &registercommand_callTypes);
		dls::topicType command_send 			= dls::topicType("command_send", new CommandSendMsgPubSubType(), &registercommand_sendTypes);
		dls::topicType command_feedback			= dls::topicType("command_feedback", new StringMsgPubSubType(), &registerstringmsgTypes);

		// simulation
		std::string simulation_time 			= "simulation_time";
		dls::topicType simulation_pause 		= dls::topicType("simulation_pause", new BoolMsgPubSubType(), &registerboolTypes);

		// development pool
		dls::topicType develop_testbench		= dls::topicType("develop_testbench", new StringMsgPubSubType(), &registerstringmsgTypes);

		// control signals
		dls::topicType desired_torques 			= dls::topicType("desired_torques", new DesiredTorquesMsgPubSubType(), &registerdesired_torquesTypes);
		dls::topicType control_signal 			= dls::topicType("control_signal", new  ControlSignalMsgPubSubType(), &registercontrol_signalTypes);
		dls::topicType gait_signal 				= dls::topicType("gaitSignal", new  GaitSignalMsgPubSubType(), &registergait_signalTypes);

		namespace high_level_estimation
		{
			dls::topicType legs_pose			= dls::topicType("legs_pose", new LegsPoseMsgPubSubType(), &registerjoint_stateTypes);
		}
			
		namespace low_level_estimation
		{
			dls::topicType blind_state 			= dls::topicType("blind_state", new BlindStateMsgPubSubType(), &registerblind_stateTypes);
			dls::topicType hyq_raw 				= dls::topicType("hyq_raw", new HyQRawMsgPubSubType(), &registerhyq_rawTypes);
			dls::topicType hyqreal_raw 			= dls::topicType("hyqreal_raw", new HyQRealRawMsgPubSubType(), &registerhyqreal_rawTypes);
            dls::topicType aliengo_raw 			= dls::topicType("aliengo_raw", new AliengoRawMsgPubSubType(), &registeraliengo_rawTypes);
			dls::topicType imu 					= dls::topicType("imu", new ImuMsgPubSubType(), &registerimu_mgxTypes);
			dls::topicType joint_states			= dls::topicType("joint_states", new JointStateMsgPubSubType(), &registerjoint_stateTypes);
		}
	}
}

#endif /* end of include guard: TOPICS_CPP */
