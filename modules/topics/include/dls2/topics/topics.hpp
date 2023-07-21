
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
#include <dls_messages/dds/gait_signalPubSubTypes.h>
#include <dls_messages/dds/timePubSubTypes.h>
#include <dls_messages/dds/boolPubSubTypes.h>
#include <dls_messages/dds/joint_statePubSubTypes.h>
#include <dls_messages/dds/hyq_rawPubSubTypes.h>
#include <dls_messages/dds/imuPubSubTypes.h>
#include <dls_messages/dds/command_sendPubSubTypes.h>
#include <dls_messages/dds/aliengo_rawPubSubTypes.h>
#include <dls_messages/dds/legs_posePubSubTypes.h>
#include <dls_messages/dds/t265_odometryPubSubTypes.h>
#include <dls_messages/dds/attitude_estimationPubSubTypes.h>
#include <dls_messages/dds/sensor_fusionPubSubTypes.h>
#include <dls_messages/dds/viconPubSubTypes.h>
#include <dls_messages/dds/param_serverPubSubTypes.h>
#include <dls_messages/dds/doublePubSubTypes.h>
#include <dls_messages/dds/controller_commandPubSubTypes.h>
#include <dls_messages/dds/t265_statePubSubTypes.h>
#include <dls_messages/dds/slip_flagPubSubTypes.h>

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
		extern dls::topicType debug_log_stream;

		// command
		extern dls::topicType command_call;
		extern dls::topicType command_send;
		extern dls::topicType command_feedback;

		// simulation
		extern dls::topicType simulation_pause;
		extern dls::topicType time_factor;

		// development pool
		extern dls::topicType develop_testbench;

		// control signals
		extern dls::topicType desired_torques;
		extern dls::topicType control_signal;
		extern dls::topicType gait_signal;
		extern dls::topicType joy_signal;
		extern dls::topicType controller_signal;

		// services
		extern dls::topicType param_server;
				
		namespace high_level_estimation
		{
			extern dls::topicType legs_pose;
			extern dls::topicType t265_odometry;
			extern dls::topicType attitude_estimation;
			extern dls::topicType sensor_fusion;
			extern dls::topicType vicon;
		}

		namespace low_level_estimation
		{
			extern dls::topicType blind_state;
			extern dls::topicType hyq_raw;
            extern dls::topicType aliengo_raw;
			extern dls::topicType imu;
			extern dls::topicType joint_states;
			extern dls::topicType t265_state;
			extern dls::topicType slip_flag;
		}
	}
}

#endif /* end of include guard: TOPICS_HPP_1NBJMKZJ */
