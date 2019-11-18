#ifndef DLS2_TO_ROSCONTROL_H
#define DLS2_TO_ROSCONTROL_H

#include <ros/node_handle.h>
#include <hardware_interface/robot_hw.h>
#include <dls_hardware_interface/joint_command_adv_interface.h>
#include <controller_interface/controller.h>
#include <pluginlib/class_list_macros.h>

// fastrtps
#include "todo.h"
#include "util/messaging/subscriber_base.hpp"
#include "msg/desired_torquesPubSubTypes.h"
#include <mutex>
#include <memory>

namespace dls2_to_roscontrol {

class Dls2ToRoscontrol : public controller_interface::Controller<hardware_interface::JointCommandAdvInterface>
{
public:
	Dls2ToRoscontrol();
	bool init(hardware_interface::JointCommandAdvInterface* hw, ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh);
	void starting(const ros::Time &time);
	void stopping(const ros::Time &time);
	void update(const ros::Time& time, const ros::Duration& period);
private:
	std::vector<hardware_interface::JointCommandAdvHandle> joint_commands_;

	class ControlMsgListener : public SubscriberBase<DesiredTorquesMsgPubSubType>
	{
	public:
		ControlMsgListener();
		std::shared_ptr<DesiredTorquesMsg> getSignal();

	private:
		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;
		eprosima::fastrtps::SampleInfo_t info;
		// BEGIN Critical section
			std::shared_ptr<DesiredTorquesMsg> pMsg;
			std::mutex msg_mutex;
		// END Critical section
	} control_signal_listener;
};

//PLUGINLIB_DECLARE_CLASS(dls2_to_roscontrol, Dls2ToRoscontrol, dls2_to_roscontrol::Dls2ToRoscontrol, controller_interface::ControllerBase);
PLUGINLIB_EXPORT_CLASS(dls2_to_roscontrol::Dls2ToRoscontrol, controller_interface::ControllerBase)

} //namespace dls2_to_roscontrol

#endif // DLS2_TO_ROSCONTROL_H
