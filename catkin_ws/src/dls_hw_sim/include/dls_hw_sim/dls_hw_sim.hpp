#ifndef GAZEBO_ROS_CONTROL___DLS_HW_SIM_H
#define GAZEBO_ROS_CONTROL___DLS_HW_SIM_H

// ROS includes
#include <ros/ros.h>
#include <std_srvs/Empty.h> // freeze base service
#include <pluginlib/class_list_macros.h>


// Gazebo includes
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/gazebo.hh>

// Gazebo ros control include
#include <gazebo_ros_control/robot_hw_sim.h>

#include <dls2_hardware_interface/dls2_robot_hw.h>

#include "dls_hw_sim/gazebo_hardware_interface.hpp"
#include "dls_hw_sim/interfaces/imu_kvh.hpp"
#include "dls_hw_sim/interfaces/imu_mgx.hpp"
#include "dls_hw_sim/interfaces/joint_state.hpp"
#include "dls_hw_sim/interfaces/imu_sensor.hpp"
#include "dls_hw_sim/interfaces/blind_state.hpp"
#include "dls_hw_sim/interfaces/hyq_raw.hpp"
#include "dls_hw_sim/interfaces/odometry.hpp"

#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/util/messaging/callback_subscriber.hpp"
#include "dls2/msg/blind_statePubSubTypes.h"
#include "dls2/msg/timePubSubTypes.h"
#include "dls2/topics/low_level_estimation/blind_state.hpp"
#include "dls2/topics/simulation_time.hpp"
#include "dls2/command/command.hpp"
#include "dls2/util/log/log.hpp"
#include "dls2/msg/desired_torquesPubSubTypes.h"
#include <mutex>


#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/msg/desired_torquesPubSubTypes.h"
#include "dls2/topics/desired_torques.hpp"
namespace dls_hw_sim
{

/**
 * @class This class implements the Gazebo hardware interface of HyQ2Max.
 * @brief This hardware interface is loaded to Gazebo using gazebo_ros_control plugin
 * which required the initSim, readSim and writeSim methods override in this class.
 */
class DlsRobotHwSim : public gazebo::ModelPlugin //public gazebo_ros_control::RobotHWSim, public hardware_interface::DlsRobotHwInterface
{
public:
	DlsRobotHwSim();

	void Load
	(
		gazebo::physics::ModelPtr,
		sdf::ElementPtr
	) override;

	void onGazeboUpdate();
  /**
     * @brief Initializes the Hy2Max hardware interface by reading the urdf file
     * @param const std::string& robot_namespace Robot namespace
     * @param ros::NodeHandle Model node handle
     * @param gazebo::physics::ModelPtr Gazebo model pointer
     * @param const urdf::Model *const URDF model
     * @param std::vector<transmission_interface::TransmissionInfo> Transmissions information
     */
  bool initSim(const std::string& robot_namespace,
               ros::NodeHandle model_nh,
               gazebo::physics::ModelPtr parent_model,
               const urdf::Model *const urdf_model,
               std::vector<transmission_interface::TransmissionInfo> transmissions);

  /**
     * @brief Reads all the sensors from Gazebo
     * @param ros::Time Simulated time
     * @param ros::Duration Simulated period
     */
  void readSim(ros::Time time, ros::Duration period);

  /**
     * @brief Writes the forces values to Gazebo
     * @param ros::Time Simulated time
     * @param ros::Duration Simulated period
     */
  void writeSim(ros::Time time, ros::Duration period);

  /**
     * @brief Handles the user commands to control the simulation environment
     */
  void simulationInterface(void);

  /**
     * @brief Toggles freeze base
     */
  bool freezeBase(std_srvs::Empty::Request& req, std_srvs::Empty::Response& res);

  bool checkForConflict(const std::list<hardware_interface::ControllerInfo>& info) const;

  void publish_blind_state();

private:

  DlsGazeboImuKvh dls_gazebo_imu_kvh_;
  DlsGazeboImuMgx dls_gazebo_imu_mgx_;
  DlsGazeboImuSensor dls_gazebo_imu_sensor_;
  DlsGazeboJointState dls_gazebo_joint_state_;
  DlsGazeboBlindState dls_gazebo_blind_state_;
  DlsGazeboHyqRaw dls_gazebo_hyq_raw_;
  DlsGazeboOdometry dls_gazebo_odometry_;
  BlindStateMsg blind_state_msg_;
  dls::PublisherBase<BlindStateMsgPubSubType> blind_state_pub_;
  std::vector<transmission_interface::TransmissionInfo> transmissions_;

  dls::CallbackSubscriber<DesiredTorquesMsgPubSubType> torque_sub;
  DesiredTorquesMsg desired_torques;


  unsigned int n_dof_;

  std::vector<std::string> joint_name_;


  std::vector<gazebo::physics::JointPtr> sim_joints_;
  gazebo::physics::ModelPtr sim_model_;
  gazebo::event::ConnectionPtr update_connection;
  gazebo::math::Pose initial_pose_;

  bool freeze_cmd_;
  bool freeze_state_;
  ros::ServiceServer freeze_base_srv_;


};

	GZ_REGISTER_MODEL_PLUGIN(DlsRobotHwSim);
}

#endif
