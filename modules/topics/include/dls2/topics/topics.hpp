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


#include "dls2/msg/command_callPubSubTypes.h"
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <utility>
#include <typeinfo> 


namespace dls
{
	namespace topics
	{
		constexpr auto activate_controller 			= "console_activate_controller";
		constexpr auto activate_gait_generator 		= "console_activate_gait_generator";
		const auto command_call 					= std::pair<std::string, eprosima::fastdds::dds::TypeSupport>("command_call", new CommandCallMsgPubSubType());
		constexpr auto console 						= "console";
		constexpr auto control_signal_base 			= "control_signal_";
		constexpr auto deactivate_controller 		= "console_deactivate_controller";
		constexpr auto deactivate_gait_generator 	= "console_deactivate_gait_generator";
		constexpr auto debug_log_stream 			= "debug_log_stream";
		constexpr auto desired_torques 				= "desired_torques";
		constexpr auto error_log_stream 			= "error_log_stream";
		constexpr auto fatal_log_stream 			= "fatal_log_stream";
		constexpr auto gait_signal 					= "GaitSignal";
		constexpr auto gait_layer  					= "GaitSignalLayer";
		constexpr auto hyqreal_raw 					= "hyqreal_raw";
		constexpr auto imu 							= "imu";
		constexpr auto info_log_stream 				= "info_log_stream";
		constexpr auto joint_states					= "joint_states";
		constexpr auto simulation_pause 			= "simulation_pause";
		constexpr auto simulation_time 				= "simulation_time";
		constexpr auto warn_log_stream 				= "warn_log_stream";
		
		namespace low_level_estimation
		{
			constexpr auto blind_state 				= "low_level_estimation/blind_state";
			constexpr auto hyq_raw 					= "low_level_estimation/hyq_raw";
			constexpr auto imu 						= "low_level_estimation/imu";
		}
	}
}

#endif /* end of include guard: TOPICS_HPP_1NBJMKZJ */
