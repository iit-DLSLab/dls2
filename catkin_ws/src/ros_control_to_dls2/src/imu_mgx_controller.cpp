#include "ros_control_to_dls2/imu_mgx_controller.hpp"

namespace ros_control_to_dls2 {

ImuMgxController::ImuMgxController()
{

}

bool ImuMgxController::init(hardware_interface::ImuMgxInterface *pImu_mgx_interface, ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh)
{
	if (!pImu_mgx_interface)
	{
		ROS_ERROR("ImuMgxInterface is a null pointer");
		return false;
	}
	return true;
}

void ImuMgxController::update(const ros::Time &time, const ros::Duration &period)
{

}

void ImuMgxController::starting(const ros::Time &time) { }
void ImuMgxController::stopping(const ros::Time &time) { }

} //namespace ros_control_to_dls2
