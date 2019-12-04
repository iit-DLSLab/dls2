#ifndef DLS2_ROBOT_HW_INTERFACE_H
#define DLS2_ROBOT_HW_INTERFACE_H

#include <ros/ros.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/imu_sensor_interface.h>
#include <hardware_interface/controller_info.h>
#include <Eigen/Dense>

#include "blind_state_interface.h"



namespace hardware_interface
{

class DlsRobotHwInterface
{
public:
	DlsRobotHwInterface(){}
	~DlsRobotHwInterface(){}

	std::string getRobotName() {return robot_name_;}

protected:

	std::string robot_name_;

	hardware_interface::BlindStateInterface blind_state_interface_;
	hardware_interface::EffortJointInterface joint_command_interface_;
	hardware_interface::ImuSensorInterface imu_sensor_interface_;
	hardware_interface::JointStateInterface joint_state_interface_;
	
	hardware_interface::BlindStateHandle::Data blindStateData;
	std::string blind_state_name_;
	//std::vector<double> blind_state_joint_state_; // same as joint_position_
	std::vector<double> blind_state_base_pose_world_;
	std::vector<double> blind_state_base_velocity_world_;
	std::vector<double> blind_state_base_acceleration_world_;

	std::vector<std::string> joint_name_;
	std::vector<double> joint_effort_command_;

	hardware_interface::ImuSensorHandle::Data imuData;
	std::string imu_name_;
	std::vector<double> imu_orientation_;
	std::vector<double> imu_orientation_covariance_;
	std::vector<double>	imu_angular_velocity_;
	std::vector<double> imu_angular_velocity_covariance_;
	std::vector<double>	imu_linear_acceleration_;
	std::vector<double> imu_linear_acceleration_covariance_;

	//std::vector<std::string> joint_name_; // same as joint_name_
	std::vector<double> joint_position_;
	std::vector<double> joint_velocity_;
	std::vector<double> joint_acceleration_;
	std::vector<double> joint_effort_;
	

	
	
	// EXTRAS******************************
	
	bool freeze_base_;
	bool pause_sim_;
	bool reset_sim_;
	Eigen::Vector3d ext_force_;
	Eigen::Vector3d ext_torque_;
	std::vector<int> joint_types_;
	std::vector<double> joint_lower_limits_;
	std::vector<double> joint_upper_limits_;
	std::vector<double> joint_effort_limits_;

	/*
	std::vector<std::string> leg_names_;
	std::vector<double> base_orientation_;
	std::vector<double>	base_ang_vel_;
	std::vector<double> base_ang_vel_old_;
	std::vector<double>	base_ang_acc_;
	std::vector<double>	base_lin_acc_;
	std::vector<double>	base_lin_pos_;
	std::vector<double>	base_lin_vel_;
	std::vector<double> base_lin_vel_old_;

	std::vector<std::string> leg_name_;
	std::vector<std::vector<double> > force_;
	std::vector<std::vector<double> > torque_;
	std::vector<std::vector<double> > normal_;
	std::deque<bool> contact_;
	std::deque<double> shin_contact_position_;*/

};

} //@namespace hardware_interface

#endif
