#ifndef HYQ_RAW_CONTROLLER_H
#define HYQ_RAW_CONTROLLER_H

#include <ros/node_handle.h>
#include <hardware_interface/robot_hw.h>
#include <controller_interface/controller.h>
#include <pluginlib/class_list_macros.h>
#include <dls2_hardware_interface/hyq_raw_interface.h>

#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/msg/hyq_rawPubSubTypes.h"
#include "dls2/msg/timePubSubTypes.h"
#include "dls2/topics/low_level_estimation/hyq_raw.hpp"
#include "dls2/topics/simulation_time.hpp"
#include "dls2/command/command.hpp"
#include "dls2/util/log/log.hpp"
#include <mutex>

#include <dls2_msgs/HyqRaw.h>
#include <rosgraph_msgs/Clock.h>

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
	hardware_interface::HyqRawHandle hyq_raw_;
	std::shared_ptr<dls::version2::PublisherBase<HyQRawMsgPubSubType>> pHyq_raw_pub_;
	//BEGIN critcal section
	std::mutex hyq_raw_msg_mutex_;
	HyQRawMsg hyq_raw_msg_;
	//END critical section
	dls::CommandManager command_manager_;
	dls::logging::coutstream scout;
	uint32_t seq_;
};

PLUGINLIB_DECLARE_CLASS(ros_control_to_dls2, HyqRawController, ros_control_to_dls2::HyqRawController, controller_interface::ControllerBase);

} // namespace ros_control_to_dls2

#endif // HYQ_RAW_CONTROLLER_H
