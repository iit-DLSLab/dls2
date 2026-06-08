
#ifndef TOPICS_HPP_1NBJMKZJ
#define TOPICS_HPP_1NBJMKZJ

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <utility>
#include <typeinfo>
#include <functional>
#include <dls_messages/dds/ArmStatePubSubTypes.hpp>
#include <dls_messages/dds/ArmTrajectoryGeneratorPubSubTypes.hpp>
#include <dls_messages/dds/AttitudePubSubTypes.hpp>
#include <dls_messages/dds/BaseStatePubSubTypes.hpp>
#include <dls_messages/dds/BlindStatePubSubTypes.hpp>
#include <dls_messages/dds/BoolPubSubTypes.hpp>
#include <dls_messages/dds/ButtonEventPubSubTypes.hpp>
#include <dls_messages/dds/CommandCallPubSubTypes.hpp>
#include <dls_messages/dds/ControllerCommandPubSubTypes.hpp>
#include <dls_messages/dds/ControlSignalPubSubTypes.hpp>
#include <dls_messages/dds/DesiredArmTorquesPubSubTypes.hpp>
#include <dls_messages/dds/DesiredTorquesPubSubTypes.hpp>
#include <dls_messages/dds/DlsHeaderPubSubTypes.hpp>
#include <dls_messages/dds/DoublePubSubTypes.hpp>
#include <dls_messages/dds/EventLogPubSubTypes.hpp>
#include <dls_messages/dds/ProcessStatusPubSubTypes.hpp>
#include <dls_messages/dds/DlsEventsPubSubTypes.hpp>
#include <dls_messages/dds/ImuPubSubTypes.hpp>
#include <dls_messages/dds/LegsPosePubSubTypes.hpp>
#include <dls_messages/dds/NavigationReferencePubSubTypes.hpp>
#include <dls_messages/dds/ParamServerPubSubTypes.hpp>
#include <dls_messages/dds/PosePubSubTypes.hpp>
#include <dls_messages/dds/ScrewPubSubTypes.hpp>
#include <dls_messages/dds/SensorFusionPubSubTypes.hpp>
#include <dls_messages/dds/StanceStatusPubSubTypes.hpp>
#include <dls_messages/dds/StateMachineMonitorPubSubTypes.hpp>
#include <dls_messages/dds/StringPubSubTypes.hpp>
#include <dls_messages/dds/TrajectoryGeneratorPubSubTypes.hpp>
#include <dls_messages/dds/Vec3PubSubTypes.hpp>
#include <dls_messages/dds/ViconPubSubTypes.hpp>


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
		extern dls::topicType process_status;
		extern dls::topicType dls_events;

		// command
		extern dls::topicType command_call;
		extern dls::topicType command_feedback;

		// simulation
		extern dls::topicType time_factor;

		// development pool
		extern dls::topicType develop_testbench;

		// control signals
		extern dls::topicType controller_command;
		extern dls::topicType desired_torques;
		extern dls::topicType control_signal;
		extern dls::topicType trajectory_generator;
		extern dls::topicType arm_trajectory_generator;

		extern dls::topicType navigation_reference;
		extern dls::topicType move_to_pos_reference;
		extern dls::topicType base_height_reference;
		extern dls::topicType base_orientation_reference;
		extern dls::topicType step_height_reference;
		extern dls::topicType button_event;

		// services
		extern dls::topicType param_server;

		// state machine
		extern dls::topicType state_machine;

		// arm controller
		extern dls::topicType desired_arm_torques;

		namespace high_level_estimation
		{
			extern dls::topicType legs_pose;
			extern dls::topicType attitude_estimation;
			extern dls::topicType sensor_fusion;
			extern dls::topicType vicon;
			extern dls::topicType base_state;
			extern dls::topicType stance_status;
		}

		namespace low_level_estimation
		{
			extern dls::topicType arm_state;
			extern dls::topicType blind_state;
			extern dls::topicType imu;
		}
	}
}

#endif /* end of include guard: TOPICS_HPP_1NBJMKZJ */
