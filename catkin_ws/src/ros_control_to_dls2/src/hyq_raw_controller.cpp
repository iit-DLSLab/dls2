#include "ros_control_to_dls2/hyq_raw_controller.hpp"

namespace ros_control_to_dls2 {

HyqRawController::HyqRawController()
{

}

bool HyqRawController::init(hardware_interface::HyqRawInterface *pHyq_raw_interface, ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh)
{
	if (!pHyq_raw_interface)
	{
		ROS_ERROR("HyqRawInterface is a null pointer");
		return false;
	}
	return true;
}

void HyqRawController::update(const ros::Time &time, const ros::Duration &period)
{

}

void HyqRawController::starting(const ros::Time &time) { }
void HyqRawController::stopping(const ros::Time &time) { }

} //namespace ros_control_to_dls2
