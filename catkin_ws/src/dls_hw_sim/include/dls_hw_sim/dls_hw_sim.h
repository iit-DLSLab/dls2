#ifndef GAZEBO_ROS_CONTROL___DLS_HW_SIM_H
#define GAZEBO_ROS_CONTROL___DLS_HW_SIM_H

// ROS includes
#include <ros/ros.h>
#include <std_srvs/Empty.h>
#include <angles/angles.h>
#include <pluginlib/class_list_macros.h>
#include <realtime_tools/realtime_publisher.h>

// Gazebo includes
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/sensors/ImuSensor.hh>
#include <gazebo/sensors/ContactSensor.hh>
#include <gazebo_msgs/ContactsState.h>
#include <std_msgs/Float64.h>

// Gazebo ros control include
#include <gazebo_ros_control/robot_hw_sim.h>
#include <hardware_interface/robot_hw.h>
#include <hardware_interface/force_torque_sensor_interface.h>
#include <hardware_interface/imu_sensor_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <dls2_hardware_interface/dls2_robot_hw.h>
#include <dls2_msgs/BlindState.h>
#include <sensor_msgs/JointState.h>
#include <nav_msgs/Odometry.h>

// URDF include
#include <urdf/model.h>

namespace dls_hw_sim
{

/**
 * @class This class implements the Gazebo hardware interface of HyQ2Max.
 * @brief This hardware interface is loaded to Gazebo using gazebo_ros_control plugin
 * which required the initSim, readSim and writeSim methods override in this class.
 */
class DlsRobotHwSim : public gazebo_ros_control::RobotHWSim, public hardware_interface::DlsRobotHwInterface
{
public:
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
     * @brief Reads all the sensors of HyQ2Max from Gazebo: encoders and imu
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

private:

	ros::Publisher joint_state_pub_;
	ros::Publisher odometry_pub_;
	ros::Publisher blind_state_pub_;
	sensor_msgs::JointState joint_state_msg_;
	nav_msgs::Odometry odometry_msg_;
	dls2_msgs::BlindState blind_state_msg_;
	void fillJointStateMsg(ros::Time time);
	void fillOdometryMsg(ros::Time time);
	void fillBlindStateMsg(ros::Time time);
	void fillJointStateMsgAndPublish(ros::Time time);
	void fillOdometryMsgAndPublish(ros::Time time);
	void fillBlindStateMsgAndPublish(ros::Time time);

	
  unsigned int n_dof_;

  std::vector<std::string> joint_name_;



  std::shared_ptr<gazebo::sensors::ImuSensor> imu_sensor_;
  std::vector<gazebo::physics::JointPtr> sim_joints_;
  gazebo::physics::ModelPtr sim_model_;
  gazebo::math::Pose initial_pose_;

  bool freeze_cmd_;
  bool freeze_state_;
  ros::ServiceServer freeze_base_srv_;

};

typedef std::shared_ptr<DlsRobotHwSim> DlsRobotHwSimPtr;

}

#endif
