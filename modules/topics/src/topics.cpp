
#ifndef TOPICS_CPP
#define TOPICS_CPP

#include "dls2/topics/topics.hpp"

namespace dls
{

	namespace topics
	{
		// logs
		dls::topicType warn_log_stream = dls::topicType("warn_log_stream", new StringMsgPubSubType());
		dls::topicType info_log_stream = dls::topicType("info_log_stream", new StringMsgPubSubType());
		dls::topicType error_log_stream = dls::topicType("error_log_stream", new StringMsgPubSubType());
		dls::topicType fatal_log_stream = dls::topicType("fatal_log_stream", new StringMsgPubSubType());
		dls::topicType debug_log_stream = dls::topicType("debug_log_stream", new StringMsgPubSubType());
		dls::topicType log_events = dls::topicType("log_events", new EventLogPubSubType());

		// command
		dls::topicType command_call = dls::topicType("command_call", new CommandCallPubSubType());
		dls::topicType command_send = dls::topicType("command_send", new CommandSendPubSubType());
		dls::topicType command_feedback = dls::topicType("command_feedback", new StringMsgPubSubType());

		// simulation
		dls::topicType time_factor = dls::topicType("time_factor", new DoubleMsgPubSubType());

		// development pool
		dls::topicType develop_testbench = dls::topicType("develop_testbench", new StringMsgPubSubType());

		// control signals
		dls::topicType desired_torques = dls::topicType("desired_torques", new DesiredTorquesPubSubType());
		dls::topicType control_signal = dls::topicType("control_signal", new ControlSignalPubSubType());
		dls::topicType trajectory_generator = dls::topicType("trajectory_generator", new TrajectoryGeneratorPubSubType());
		dls::topicType joy_signal = dls::topicType("joy_signal", new ControllerCommandPubSubType());
		dls::topicType controller_signal = dls::topicType("controller_command", new ControllerCommandPubSubType());
		dls::topicType mpc_generator_output = dls::topicType("mpc_generator_output", new MPCGeneratorOutputPubSubType());

		// services
		dls::topicType param_server = dls::topicType("add_double", new ParamServerPubSubType());

		// visualizations

		dls::topicType sphere_list = dls::topicType("sphere_list", new SphereListPubSubType());
		dls::topicType arrow_list = dls::topicType("arrow_list", new ArrowListPubSubType());
		dls::topicType friction_cone_list = dls::topicType("friction_cone_list", new FrictionConeListPubSubType());

		dls::topicType generic_robot = dls::topicType("generic_robot", new GenericRobotPubSubType());


		// state machine

		dls::topicType state_machine = dls::topicType("state_machine", new StateMachineMonitorPubSubType());

		namespace high_level_estimation
		{
			dls::topicType legs_pose = dls::topicType("legs_pose", new LegsPosePubSubType());
			dls::topicType t265_odometry = dls::topicType("t265_odometry", new T265OdometryPubSubType());
			dls::topicType attitude_estimation = dls::topicType("attitude_estimation", new AttitudePubSubType());
			dls::topicType sensor_fusion = dls::topicType("sensor_fusion", new SensorFusionPubSubType());
			dls::topicType vicon = dls::topicType("vicon", new ViconPubSubType());
			dls::topicType base_state = dls::topicType("base_state", new BaseStatePubSubType());
			dls::topicType stance_status = dls::topicType("stance_status", new StanceStatusPubSubType());
		}

		namespace low_level_estimation
		{
			dls::topicType blind_state = dls::topicType("blind_state", new BlindStatePubSubType());
			dls::topicType imu = dls::topicType("imu", new ImuPubSubType());
			dls::topicType slip_flag = dls::topicType("slip_flag", new SlipFlagPubSubType());
		}
	}
}

#endif /* end of include guard: TOPICS_CPP */
