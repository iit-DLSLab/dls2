#include "ros_control_to_dls2/imu_kvh_controller.hpp"

namespace ros_control_to_dls2 {

ImuKvhController::ImuKvhController()
{

}

bool ImuKvhController::init(hardware_interface::ImuKvhInterface *pImu_kvh_interface, ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh)
{
	if (!pImu_kvh_interface)
	{
		ROS_ERROR("ImuKvhInterface is a null pointer");
		return false;
	}
	return true;
}

void ImuKvhController::update(const ros::Time &time, const ros::Duration &period)
{

}

void ImuKvhController::starting(const ros::Time &time) { }
void ImuKvhController::stopping(const ros::Time &time) { }

} //namespace ros_control_to_dls2
