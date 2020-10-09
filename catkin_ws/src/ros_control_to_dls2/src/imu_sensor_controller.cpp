#include "ros_control_to_dls2/imu_sensor_controller.hpp"

namespace ros_control_to_dls2 {

ImuSensorController::ImuSensorController() :
	command_manager_(),
	scout("imu")
{
	std::cout << "Constructed ImuController" << std::endl;
	command_manager_.addCommand<void,dls::ARGVOID>
	(
		"imu",
		"where",
		"Displays the IMU values",
		std::function<void(dls::ARGVOID)>
		(
			[&](dls::ARGVOID)
			{
				std::lock_guard<std::mutex> lock(this->imu_msg_mutex_);
				this->scout << "IMU" << std::endl;
			}
		)
	);
}

bool ImuSensorController::init(hardware_interface::ImuSensorInterface *pImu_sensor_interface, ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh)
{
	if (!pImu_sensor_interface)
	{
		ROS_ERROR("ImuSensorInterface is a null pointer");
		return false;
	}
	std::lock_guard<std::mutex> lock(this->imu_msg_mutex_);
	pImu_sensor_pub_ = std::make_shared<dls::PublisherBase<ImuMsgPubSubType>>(dls::topics::imu);
	seq_=0;
	imu_sensor_ = pImu_sensor_interface->getHandle("trunk_imu");
	return true;
}

void ImuSensorController::update(const ros::Time &time, const ros::Duration &period)
{
	std::lock_guard<std::mutex> lock(this->imu_msg_mutex_);
}

void ImuSensorController::starting(const ros::Time &time) { }
void ImuSensorController::stopping(const ros::Time &time) { }

} //namespace ros_control_to_dls2
