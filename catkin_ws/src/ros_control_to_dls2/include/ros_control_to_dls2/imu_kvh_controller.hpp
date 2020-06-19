#ifndef IMU_KVH_CONTROLLER_H
#define IMU_KVH_CONTROLLER_H

#include <ros/node_handle.h>
#include <hardware_interface/robot_hw.h>
#include <controller_interface/controller.h>
#include <pluginlib/class_list_macros.h>
#include <dls2_hardware_interface/imu_kvh_interface.h>

namespace ros_control_to_dls2 {

class ImuKvhController : public controller_interface::Controller<hardware_interface::ImuKvhInterface>
{
public:
	ImuKvhController();
	bool init(hardware_interface::ImuKvhInterface* hw, ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh);
	void starting(const ros::Time &time);
	void stopping(const ros::Time &time);
	void update(const ros::Time& time, const ros::Duration& period);
private:
	std::vector<hardware_interface::ImuKvhHandle> imu_kvh_;
};

PLUGINLIB_DECLARE_CLASS(ros_control_to_dls2, ImuKvhController, ros_control_to_dls2::ImuKvhController, controller_interface::ControllerBase);

} // namespace ros_control_to_dls2

#endif // IMU_KVH_CONTROLLER_H
