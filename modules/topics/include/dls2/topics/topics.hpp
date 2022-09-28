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
#ifndef TOPICS_HPP_1NBJMKZJ
#define TOPICS_HPP_1NBJMKZJ

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <utility>
#include <typeinfo>
#include <functional> 
#include <dls_messages/dds/command_callPubSubTypes.h>
#include <dls_messages/dds/stringmsgPubSubTypes.h>
#include <dls_messages/dds/hyqreal_rawPubSubTypes.h>
#include <dls_messages/dds/desired_torquesPubSubTypes.h>
#include <dls_messages/dds/control_signalPubSubTypes.h>
#include <dls_messages/dds/blind_statePubSubTypes.h>
#include <dls_messages/dds/gait_signalPubSubTypes.h>
#include <dls_messages/dds/timePubSubTypes.h>
#include <dls_messages/dds/boolPubSubTypes.h>
#include <dls_messages/dds/joint_statePubSubTypes.h>
#include <dls_messages/dds/hyq_rawPubSubTypes.h>
#include <dls_messages/dds/imuPubSubTypes.h>
#include <dls_messages/dds/command_sendPubSubTypes.h>
#include <dls_messages/dds/aliengo_rawPubSubTypes.h>

namespace dls
{
	typedef std::tuple<std::string, eprosima::fastdds::dds::TypeSupport, std::function<void()>> topicType;

	namespace topics
	{
		// logs
		extern dls::topicType warn_log_stream;
		extern dls::topicType info_log_stream;
		extern dls::topicType error_log_stream;
		extern dls::topicType fatal_log_stream;
		extern dls::topicType debug_log_stream;

		// command
		extern dls::topicType command_call;
		extern std::string console;

		// simulation
		extern std::string simulation_time;
		extern dls::topicType simulation_pause;

		// development pool
		extern dls::topicType develop_testbench;

		// control signals
		extern dls::topicType desired_torques;
		extern dls::topicType control_signal;
		extern dls::topicType gait_signal;
		extern dls::topicType joint_states;

		// commands send
		extern dls::topicType command_send;

		
		namespace low_level_estimation
		{
			extern dls::topicType blind_state;
			extern dls::topicType hyq_raw;
			extern dls::topicType hyqreal_raw;
            extern dls::topicType aliengo_raw;
			extern dls::topicType imu;
		}
	}
}

#endif /* end of include guard: TOPICS_HPP_1NBJMKZJ */
