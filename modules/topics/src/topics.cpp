
#ifndef TOPICS_CPP
#define TOPICS_CPP

#include "dls2/topics/topics.hpp"

namespace dls
{

  namespace topics
  {
    // logs
    dls::topicType warn_log_stream = dls::topicType("warn_log_stream",
        new dls2_interface::msg::StringPubSubType());
    dls::topicType info_log_stream = dls::topicType("info_log_stream",
        new dls2_interface::msg::StringPubSubType());
    dls::topicType error_log_stream = dls::topicType("error_log_stream",
        new dls2_interface::msg::StringPubSubType());
    dls::topicType fatal_log_stream = dls::topicType("fatal_log_stream",
        new dls2_interface::msg::StringPubSubType());
    dls::topicType debug_log_stream = dls::topicType("debug_log_stream",
        new dls2_interface::msg::StringPubSubType());
    dls::topicType log_events = dls::topicType("log_events",
        new dls2_interface::msg::EventLogPubSubType());
    dls::topicType process_status = dls::topicType("process_status", 
        new dls2_interface::msg::ProcessStatusPubSubType());
    dls::topicType dls_events = dls::topicType("dls/events", 
        new dls2_interface::msg::DlsEventsPubSubType());

    // CS-OBSW related topics
    dls::topicType dls_status = dls::topicType("dls/status", 
        new dls2_interface::msg::DlsStatusPubSubType());

    dls::topicType cs_basic_command = dls::topicType("cs/basic_command", 
        new dls2_interface::msg::CSBasicCommandPubSubType());
    dls::topicType cs_emergency = dls::topicType("cs/emergency", 
        new dls2_interface::msg::CSEmergencyPubSubType());
    dls::topicType cs_target_position = dls::topicType("cs/target_position", 
        new dls2_interface::msg::TargetPositionPubSubType());
    dls::topicType cs_stay_out_zones = dls::topicType("cs/stay_out_zones", 
        new dls2_interface::msg::StayOutZonesPubSubType());
    dls::topicType cs_reference_path = dls::topicType("cs/reference_path", 
        new dls2_interface::msg::ReferencePathPubSubType());
    dls::topicType cs_feet_reference = dls::topicType("cs/feet_reference", 
        new dls2_interface::msg::FeetReferencePubSubType());
    dls::topicType cs_base_reference = dls::topicType("cs/base_reference", 
        new dls2_interface::msg::BaseReferencePubSubType());
    dls::topicType cs_joint_states = dls::topicType("cs/joint_reference", 
        new dls2_interface::msg::JointStatesPubSubType());
    dls::topicType cs_loc_reset = dls::topicType("cs/loc_reset", 
        new dls2_interface::msg::BaseReferencePubSubType());
    dls::topicType cs_command_call = dls::topicType("cs/command_call", 
        new dls2_interface::msg::CommandCallPubSubType());

    // command
    dls::topicType command_call = dls::topicType("command_call",
        new dls2_interface::msg::CommandCallPubSubType());
    dls::topicType command_feedback = dls::topicType("command_feedback",
        new dls2_interface::msg::StringPubSubType());

    // simulation
    dls::topicType time_factor = dls::topicType("time_factor",
        new dls2_interface::msg::DoublePubSubType());

    // development pool
    dls::topicType develop_testbench = dls::topicType("develop_testbench",
        new dls2_interface::msg::StringPubSubType());

    // control signals
    dls::topicType controller_command = dls::topicType("controller_command",
        new dls2_interface::msg::ControllerCommandPubSubType());
    dls::topicType desired_torques = dls::topicType("desired_torques",
        new dls2_interface::msg::DesiredTorquesPubSubType());
    dls::topicType control_signal = dls::topicType("control_signal",
        new dls2_interface::msg::ControlSignalPubSubType());
    dls::topicType trajectory_generator = dls::topicType("trajectory_generator",
        new dls2_interface::msg::TrajectoryGeneratorPubSubType());
    dls::topicType navigation_reference = dls::topicType("navigation_reference",
        new dls2_interface::msg::NavigationReferencePubSubType());
    dls::topicType button_event = dls::topicType("button_event",
        new dls2_interface::msg::ButtonEventPubSubType());

    // services
    dls::topicType param_server = dls::topicType("add_double",
        new dls2_interface::msg::ParamServerPubSubType());

    // state machine
    dls::topicType state_machine = dls::topicType("state_machine",
        new dls2_interface::msg::StateMachineMonitorPubSubType());

    // arm controller
    dls::topicType desired_arm_torques = dls::topicType("desired_arm_torques",
        new dls2_interface::msg::DesiredArmTorquesPubSubType());
    dls::topicType arm_trajectory_generator = dls::topicType("arm_trajectory_generator",
        new dls2_interface::msg::ArmTrajectoryGeneratorPubSubType());

    namespace high_level_estimation
    {
      dls::topicType legs_pose = dls::topicType("legs_pose",
          new dls2_interface::msg::LegsPosePubSubType());
      dls::topicType attitude_estimation = dls::topicType("attitude_estimation",
          new dls2_interface::msg::AttitudePubSubType());
      dls::topicType sensor_fusion = dls::topicType("sensor_fusion",
          new dls2_interface::msg::SensorFusionPubSubType());
      dls::topicType vicon = dls::topicType("vicon",
          new dls2_interface::msg::ViconPubSubType());
      dls::topicType base_state = dls::topicType("base_state",
          new dls2_interface::msg::BaseStatePubSubType());
      dls::topicType stance_status = dls::topicType("stance_status",
          new dls2_interface::msg::StanceStatusPubSubType());
    }

    namespace low_level_estimation
    {
      dls::topicType arm_state = dls::topicType("arm_state",
          new dls2_interface::msg::ArmStatePubSubType());
      dls::topicType blind_state = dls::topicType("blind_state",
          new dls2_interface::msg::BlindStatePubSubType());
      dls::topicType imu = dls::topicType("imu",
          new dls2_interface::msg::ImuPubSubType());
    }
  }
}

#endif /* end of include guard: TOPICS_CPP */
