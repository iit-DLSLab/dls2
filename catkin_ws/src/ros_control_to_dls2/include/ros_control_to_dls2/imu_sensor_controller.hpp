#ifndef IMU_SENSOR_CONTROLLER_H
#define IMU_SENSOR_CONTROLLER_H

#include <ros/node_handle.h>
#include <hardware_interface/robot_hw.h>
#include <controller_interface/controller.h>
#include <pluginlib/class_list_macros.h>
#include <hardware_interface/imu_sensor_interface.h>

#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/msg/imuPubSubTypes.h"
#include "dls2/msg/timePubSubTypes.h"
#include "dls2/msg/imu.h"
#include "dls2/topics/imu.hpp"
#include "dls2/topics/simulation_time.hpp"
#include "dls2/command/command.hpp"
#include "dls2/util/log/log.hpp"
#include <mutex>

#include <rosgraph_msgs/Clock.h>

namespace ros_control_to_dls2 {

class ImuSensorController : public controller_interface::Controller<hardware_interface::ImuSensorInterface>
{
public:
	ImuSensorController();
	bool init(hardware_interface::ImuSensorInterface* hw, ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh);
	void starting(const ros::Time &time);
	void stopping(const ros::Time &time);
	void update(const ros::Time& time, const ros::Duration& period);
private:
	hardware_interface::ImuSensorHandle imu_sensor_;
	std::shared_ptr<dls::PublisherBase<ImuMsgPubSubType>> pImu_sensor_pub_;
	// BEGIN critical section
	std::mutex imu_msg_mutex_;
	ImuMsg imu_sensor_msg_;
	// END critical section
	dls::CommandManager command_manager_;
	dls::logging::coutstream scout;
	uint32_t seq_;
};

PLUGINLIB_DECLARE_CLASS(ros_control_to_dls2, ImuSensorController, ros_control_to_dls2::ImuSensorController, controller_interface::ControllerBase);

} // namespace ros_control_to_dls2

#endif // IMU_SENSOR_CONTROLLER_H
