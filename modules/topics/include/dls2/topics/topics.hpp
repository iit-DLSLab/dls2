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
#include "dls2/msg/command_callPubSubTypes.h"
#include "dls2/msg/stringmsgPubSubTypes.h"
#include "dls2/msg/hyqreal_rawPubSubTypes.h"
#include "dls2/msg/command_registerPubSubTypes.h"
#include "dls2/msg/desired_torquesPubSubTypes.h"
#include "dls2/msg/control_signalPubSubTypes.h"
#include "dls2/msg/blind_statePubSubTypes.h"
#include "dls2/msg/gait_signalPubSubTypes.h"
#include "dls2/msg/timePubSubTypes.h"
#include "dls2/msg/boolPubSubTypes.h"
#include "dls2/msg/joint_statePubSubTypes.h"
#include "dls2/msg/hyq_rawPubSubTypes.h"
#include "dls2/msg/imuPubSubTypes.h"


namespace dls
{
	typedef std::pair<std::string, eprosima::fastdds::dds::TypeSupport> topicType;

	namespace topics
	{
		// logs
		extern dls::topicType warn_log_stream;
		extern dls::topicType info_log_stream;
		extern dls::topicType error_log_stream;
		extern dls::topicType fatal_log_stream;
		extern dls::topicType hyqreal_raw;
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
		extern std::string gait_layer;


		extern std::string activate_controller;
		extern std::string activate_gait_generator;
		extern std::string deactivate_controller;
		extern std::string deactivate_gait_generator;
				
		
		namespace low_level_estimation
		{
			extern dls::topicType blind_state;
			extern dls::topicType hyq_raw;
			extern dls::topicType imu;
		}
	}
}

#endif /* end of include guard: TOPICS_HPP_1NBJMKZJ */
