#ifndef DLS2_TO_ROSCONTROL_H
#define DLS2_TO_ROSCONTROL_H

#include <ros/node_handle.h>
#include <hardware_interface/robot_hw.h>
#include <hardware_interface/joint_command_interface.h>
#include <controller_interface/controller.h>
#include <dls2/controller/control_signal.hpp>
#include <pluginlib/class_list_macros.h>

// fastrtps
// #include "todo.h"
#include "dls2/util/messaging/subscriber_base.hpp"
#include "dls2/util/messaging/publisher_base.hpp"
//#include "msg/desired_torquesPubSubTypes.h"
// #include "msg/control_signalPubSubTypes.h"
#include "dls2/msg/desired_torquesPubSubTypes.h"
#include "dls2/msg/joint_statePubSubTypes.h"
#include <mutex>
#include <memory>

namespace dls2_to_roscontrol {

class Dls2ToRoscontrol : public controller_interface::Controller<hardware_interface::EffortJointInterface>
{
public:
	Dls2ToRoscontrol();
	bool init(hardware_interface::EffortJointInterface* hw, ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh);
	void starting(const ros::Time &time);
	void stopping(const ros::Time &time);
	void update(const ros::Time& time, const ros::Duration& period);
private:
	std::vector<hardware_interface::JointHandle> joint_commands_;

	class ControlMsgListener : public dls::SubscriberBase<DesiredTorquesMsgPubSubType>
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

	dls::PublisherBase<JointStateMsgPubSubType> joint_state_pub_;
};

//PLUGINLIB_DECLARE_CLASS(dls2_to_roscontrol, Dls2ToRoscontrol, dls2_to_roscontrol::Dls2ToRoscontrol, controller_interface::ControllerBase);
PLUGINLIB_EXPORT_CLASS(dls2_to_roscontrol::Dls2ToRoscontrol, controller_interface::ControllerBase)

} //namespace dls2_to_roscontrol

#endif // DLS2_TO_ROSCONTROL_H
