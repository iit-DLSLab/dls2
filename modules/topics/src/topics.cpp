
#ifndef TOPICS_CPP
#define TOPICS_CPP

#include "dls2/topics/topics.hpp"

namespace dls
{

	namespace topics
	{
		// logs
		dls::topicType warn_log_stream = dls::topicType("warn_log_stream", new dls2_interface::msg::StringMsgPubSubType());
		dls::topicType info_log_stream = dls::topicType("info_log_stream", new dls2_interface::msg::StringMsgPubSubType());
		dls::topicType error_log_stream = dls::topicType("error_log_stream", new dls2_interface::msg::StringMsgPubSubType());
		dls::topicType fatal_log_stream = dls::topicType("fatal_log_stream", new dls2_interface::msg::StringMsgPubSubType());
		dls::topicType debug_log_stream = dls::topicType("debug_log_stream", new dls2_interface::msg::StringMsgPubSubType());
		dls::topicType log_events = dls::topicType("log_events", new dls2_interface::msg::EventLogPubSubType());

		// command
		dls::topicType command_call = dls::topicType("command_call", new dls2_interface::msg::CommandCallPubSubType());
		dls::topicType command_send = dls::topicType("command_send", new dls2_interface::msg::CommandSendPubSubType());
		dls::topicType command_feedback = dls::topicType("command_feedback", new dls2_interface::msg::StringMsgPubSubType());

		// simulation
		dls::topicType time_factor = dls::topicType("time_factor", new dls2_interface::msg::DoubleMsgPubSubType());

		// development pool
		dls::topicType develop_testbench = dls::topicType("develop_testbench", new dls2_interface::msg::StringMsgPubSubType());

		// control signals
		dls::topicType desired_torques = dls::topicType("desired_torques", new dls2_interface::msg::DesiredTorquesPubSubType());
		dls::topicType control_signal = dls::topicType("control_signal", new dls2_interface::msg::ControlSignalPubSubType());
		dls::topicType trajectory_generator = dls::topicType("trajectory_generator", new dls2_interface::msg::TrajectoryGeneratorPubSubType());
		dls::topicType joy_signal = dls::topicType("joy_signal", new dls2_interface::msg::ControllerCommandPubSubType());
		dls::topicType controller_signal = dls::topicType("controller_command", new dls2_interface::msg::ControllerCommandPubSubType());
		dls::topicType mpc_generator_output = dls::topicType("mpc_generator_output", new dls2_interface::msg::MPCGeneratorOutputPubSubType());
    dls::topicType navigation_reference = dls::topicType("navigation_reference",
        new dls2_interface::msg::NavigationReferencePubSubType());
    dls::topicType button_event = dls::topicType("button_event",
        new dls2_interface::msg::ButtonEventPubSubType());
    dls::topicType operational_mode = dls::topicType("operational_mode",
        new dls2_interface::msg::OperationalModePubSubType());

		// services
		dls::topicType param_server = dls::topicType("add_double", new dls2_interface::msg::ParamServerPubSubType());

		// visualizations

		dls::topicType sphere_list = dls::topicType("sphere_list", new dls2_interface::msg::SphereListPubSubType());
		dls::topicType arrow_list = dls::topicType("arrow_list", new dls2_interface::msg::ArrowListPubSubType());
		dls::topicType friction_cone_list = dls::topicType("friction_cone_list", new dls2_interface::msg::FrictionConeListPubSubType());

		dls::topicType generic_robot = dls::topicType("generic_robot", new dls2_interface::msg::GenericRobotPubSubType());


		// state machine

		dls::topicType state_machine = dls::topicType("state_machine", new dls2_interface::msg::StateMachineMonitorPubSubType());

		// arm controller
		dls::topicType desired_arm_torques = dls::topicType("desired_arm_torques", new dls2_interface::msg::DesiredArmTorquesPubSubType());
		dls::topicType arm_trajectory_generator = dls::topicType("arm_trajectory_generator", new dls2_interface::msg::ArmTrajectoryGeneratorPubSubType());
		namespace high_level_estimation
		{
			dls::topicType legs_pose = dls::topicType("legs_pose", new dls2_interface::msg::LegsPosePubSubType());
			dls::topicType t265_odometry = dls::topicType("t265_odometry", new dls2_interface::msg::T265OdometryPubSubType());
			dls::topicType attitude_estimation = dls::topicType("attitude_estimation", new dls2_interface::msg::AttitudePubSubType());
			dls::topicType sensor_fusion = dls::topicType("sensor_fusion", new dls2_interface::msg::SensorFusionPubSubType());
			dls::topicType vicon = dls::topicType("vicon", new dls2_interface::msg::ViconPubSubType());
			dls::topicType base_state = dls::topicType("base_state", new dls2_interface::msg::BaseStatePubSubType());
			dls::topicType stance_status = dls::topicType("stance_status", new dls2_interface::msg::StanceStatusPubSubType());
		}

		namespace low_level_estimation
		{
      dls::topicType arm_state = dls::topicType("arm_state",
          new dls2_interface::msg::ArmStatePubSubType());
			dls::topicType blind_state = dls::topicType("blind_state", new dls2_interface::msg::BlindStatePubSubType());
			dls::topicType imu = dls::topicType("imu", new dls2_interface::msg::ImuPubSubType());
			dls::topicType slip_flag = dls::topicType("slip_flag", new dls2_interface::msg::SlipFlagPubSubType());
			dls::topicType arm_blind_state = dls::topicType("arm_blind_state", new dls2_interface::msg::ArmBlindStatePubSubType());
		}
	}
}

#endif /* end of include guard: TOPICS_CPP */
