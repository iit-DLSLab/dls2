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


	std::vector<std::string> joint_name_;
	std::vector<double> joint_effort_command_;




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


};

} //@namespace hardware_interface

#endif
