#include "ros_control_to_dls2/hyq_raw_controller.hpp"
#include <algorithm>
#include <iostream>

namespace ros_control_to_dls2 {

HyqRawController::HyqRawController() :
	command_manager_(),
	scout("hyq_raw")
{
	std::cout << "Constructed HyqRawController" << std::endl;
	command_manager_.addCommand<void, dls::ARGVOID>
	("hyq_raw","where","Displays the raw hyq state",
		std::function<void(dls::ARGVOID)>
		(
			[&](dls::ARGVOID)
			{
				std::lock_guard<std::mutex> lock(this->hyq_raw_msg_mutex_);
				this->scout << "Hyq Raw: ";
				// ...
				this->scout << std::endl;
			}
		)
	);
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
