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


namespace dls
{
	typedef std::pair<std::string, eprosima::fastdds::dds::TypeSupport> topicType;

	namespace topics
	{
		extern std::string activate_controller;
		extern std::string activate_gait_generator;
		extern topicType command_call;
		extern std::string console;
		extern std::string control_signal_base;
		extern std::string deactivate_controller;
		extern std::string deactivate_gait_generator;
		extern topicType debug_log_stream;
		extern std::string desired_torques;
		extern topicType error_log_stream;
		extern topicType fatal_log_stream;
		extern std::string gait_signal;
		extern std::string gait_layer;
		extern topicType hyqreal_raw;
		extern topicType info_log_stream;
		extern std::string joint_states;
		extern std::string simulation_pause;
		extern std::string simulation_time;
		extern topicType warn_log_stream;
		
		namespace low_level_estimation
		{
			extern std::string blind_state;
			extern std::string hyq_raw;
			extern std::string imu;
		}
	}
}

#endif /* end of include guard: TOPICS_HPP_1NBJMKZJ */
