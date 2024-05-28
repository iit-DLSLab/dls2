
#ifndef TOPICS_HPP_1NBJMKZJ
#define TOPICS_HPP_1NBJMKZJ

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <utility>
#include <typeinfo>
#include <functional>
#include <dls_messages/dds/command_callPubSubTypes.h>
#include <dls_messages/dds/stringmsgPubSubTypes.h>
#include <dls_messages/dds/desired_torquesPubSubTypes.h>
#include <dls_messages/dds/control_signalPubSubTypes.h>
#include <dls_messages/dds/blind_statePubSubTypes.h>
#include <dls_messages/dds/base_statePubSubTypes.h>
#include <dls_messages/dds/trajectory_generatorPubSubTypes.h>
#include <dls_messages/dds/imuPubSubTypes.h>
#include <dls_messages/dds/command_sendPubSubTypes.h>
#include <dls_messages/dds/legs_posePubSubTypes.h>
#include <dls_messages/dds/t265_odometryPubSubTypes.h>
#include <dls_messages/dds/attitudePubSubTypes.h>
#include <dls_messages/dds/sensor_fusionPubSubTypes.h>
#include <dls_messages/dds/viconPubSubTypes.h>
#include <dls_messages/dds/stance_statusPubSubTypes.h>
#include <dls_messages/dds/param_serverPubSubTypes.h>
#include <dls_messages/dds/doublePubSubTypes.h>
#include <dls_messages/dds/controller_commandPubSubTypes.h>
#include <dls_messages/dds/slip_flagPubSubTypes.h>
#include <dls_messages/dds/mpc_generator_outputPubSubTypes.h>
#include <dls_messages/dds/state_machinePubSubTypes.h>

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
