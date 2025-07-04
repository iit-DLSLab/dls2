
#ifndef TOPICS_HPP_1NBJMKZJ
#define TOPICS_HPP_1NBJMKZJ

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <utility>
#include <typeinfo>
#include <functional>
#include <dls_messages/dds/command_callPubSubTypes.hpp>
#include <dls_messages/dds/stringmsgPubSubTypes.hpp>
#include <dls_messages/dds/desired_torquesPubSubTypes.hpp>
#include <dls_messages/dds/control_signalPubSubTypes.hpp>
#include <dls_messages/dds/blind_statePubSubTypes.hpp>
#include <dls_messages/dds/base_statePubSubTypes.hpp>
#include <dls_messages/dds/trajectory_generatorPubSubTypes.hpp>
#include <dls_messages/dds/imuPubSubTypes.hpp>
#include <dls_messages/dds/command_sendPubSubTypes.hpp>
#include <dls_messages/dds/legs_posePubSubTypes.hpp>
#include <dls_messages/dds/t265_odometryPubSubTypes.hpp>
#include <dls_messages/dds/attitudePubSubTypes.hpp>
#include <dls_messages/dds/sensor_fusionPubSubTypes.hpp>
#include <dls_messages/dds/viconPubSubTypes.hpp>
#include <dls_messages/dds/stance_statusPubSubTypes.hpp>
#include <dls_messages/dds/param_serverPubSubTypes.hpp>
#include <dls_messages/dds/doublePubSubTypes.hpp>
#include <dls_messages/dds/controller_commandPubSubTypes.hpp>
#include <dls_messages/dds/slip_flagPubSubTypes.hpp>
#include <dls_messages/dds/mpc_generator_outputPubSubTypes.hpp>
#include <dls_messages/dds/state_machine_monitorPubSubTypes.hpp>
#include <dls_messages/dds/ArrowListPubSubTypes.hpp>
#include <dls_messages/dds/SphereListPubSubTypes.hpp>
#include <dls_messages/dds/FrictionConeListPubSubTypes.hpp>
#include <dls_messages/dds/GenericRobotPubSubTypes.hpp>
#include <dls_messages/dds/event_logPubSubTypes.hpp>


#include "dls2/topics/utils.hpp"

namespace dls
{
	namespace topics
	{
		// logs
		extern dls::topicType warn_log_stream;
		extern dls::topicType info_log_stream;
		extern dls::topicType error_log_stream;
		extern dls::topicType fatal_log_stream;
		extern dls::topicType debug_log_stream;
		extern dls::topicType log_events;

		// command
		extern dls::topicType command_call;
		extern dls::topicType command_send;
		extern dls::topicType command_feedback;

		// simulation
		extern dls::topicType time_factor;

		// development pool
		extern dls::topicType develop_testbench;

		// control signals
		extern dls::topicType desired_torques;
		extern dls::topicType control_signal;
		extern dls::topicType trajectory_generator;
		extern dls::topicType joy_signal;
		extern dls::topicType controller_signal;
		extern dls::topicType mpc_generator_output;

		// services
		extern dls::topicType param_server;

		//visualizer
		extern dls::topicType sphere_list;
		extern dls::topicType arrow_list;
		extern dls::topicType friction_cone_list;

		extern dls::topicType generic_robot;

		// state machine
		extern dls::topicType state_machine;

		namespace high_level_estimation
		{
			extern dls::topicType legs_pose;
			extern dls::topicType t265_odometry;
			extern dls::topicType attitude_estimation;
			extern dls::topicType sensor_fusion;
			extern dls::topicType vicon;
			extern dls::topicType base_state;
			extern dls::topicType stance_status;
		}

		namespace low_level_estimation
		{
			extern dls::topicType blind_state;
			extern dls::topicType imu;
			extern dls::topicType slip_flag;
		}
	}
}

#endif /* end of include guard: TOPICS_HPP_1NBJMKZJ */
