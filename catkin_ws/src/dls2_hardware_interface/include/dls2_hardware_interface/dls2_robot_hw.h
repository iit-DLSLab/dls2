#ifndef DLS2_ROBOT_HW_INTERFACE_H
#define DLS2_ROBOT_HW_INTERFACE_H

#include <ros/ros.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/imu_sensor_interface.h>
#include <hardware_interface/controller_info.h>
#include <Eigen/Dense>

#include "blind_state_interface.h"
#include "imu_kvh_interface.h"
#include "imu_mgx_interface.h"
#include "hyq_raw_interface.h"
//#include "hyqreal_raw_interface.h"

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
	hardware_interface::ImuMgxInterface imu_mgx_interface_;
	hardware_interface::ImuKvhInterface imu_kvh_interface_;
	hardware_interface::HyqRawInterface hyq_raw_interface_;
	//hardware_interface::HyqrealRawInterface hyqreal_raw_interface_;

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

	hardware_interface::ImuKvhHandle::Data imuKvhData;
	std::string					imu_kvh_name_;
	std::vector<float> 	imu_kvh_angular_velocity_;
	std::vector<float> 	imu_kvh_specific_force_;
	uint32_t 						imu_kvh_time_stamp_;
	uint8_t 						imu_kvh_seq_;
	uint8_t 						imu_kvh_status_;
	uint16_t 						imu_kvh_time_sync_;

	hardware_interface::ImuMgxHandle::Data imuMgxData;
	std::string					imu_mgx_name_;
	std::vector<float> 	imu_mgx_angular_velocity_;
	std::vector<float> 	imu_mgx_specific_force_;
	std::vector<float>	imu_mgx_quaternion_;
	uint32_t 						imu_mgx_time_stamp_;
	uint16_t 						imu_mgx_temperature_;

	hardware_interface::HyqRawHandle::Data hyqRawData;
	std::string					hyq_raw_name_;
	//ImuMgxHandle::Data hyq_imu_mgx_data_; //same as above
  //ImuKvhHandle::Data hyq_imu_jvh_data_; //same as above
	std::vector<uint32_t> hyq_abs_enc_;
	std::vector<uint32_t> hyq_rel_enc_;
	std::vector<uint16_t> hyq_torque_sensor_haa_;
	std::vector<uint16_t> hyq_load_cell_hfe_;
	std::vector<uint16_t> hyq_load_cell_kfe_;



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
