#ifndef HYQ_RAW_CONTROLLER_H
#define HYQ_RAW_CONTROLLER_H

#include <ros/node_handle.h>
#include <hardware_interface/robot_hw.h>
#include <controller_interface/controller.h>
#include <pluginlib/class_list_macros.h>
#include <dls2_hardware_interface/hyq_raw_interface.h>

namespace ros_control_to_dls2 {

class HyqRawController : public controller_interface::Controller<hardware_interface::HyqRawInterface>
{
public:
	HyqRawController();
	bool init(hardware_interface::HyqRawInterface* hw, ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh);
	void starting(const ros::Time &time);
	void stopping(const ros::Time &time);
	void update(const ros::Time& time, const ros::Duration& period);
private:
	std::vector<hardware_interface::HyqRawHandle> hyq_raw_;
};

PLUGINLIB_DECLARE_CLASS(ros_control_to_dls2, HyqRawController, ros_control_to_dls2::HyqRawController, controller_interface::ControllerBase);

} // namespace ros_control_to_dls2

#endif // HYQ_RAW_CONTROLLER_H
