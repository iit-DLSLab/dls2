#ifndef BLIND_STATE_CONTROLLER_H
#define BLIND_STATE_CONTROLLER_H

#include <ros/node_handle.h>
#include <hardware_interface/robot_hw.h>
#include <controller_interface/controller.h>
#include <pluginlib/class_list_macros.h>
#include <dls2_hardware_interface/blind_state_interface.h>

#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/msg/blind_statePubSubTypes.h"
#include "dls2/msg/timePubSubTypes.h"
#include "dls2/topics/low_level_estimation/blind_state.hpp"
#include "dls2/topics/simulation_time.hpp"

#include <dls2_msgs/BlindState.h>
#include <rosgraph_msgs/Clock.h>

namespace ros_control_to_dls2 {

class BlindStateController : public controller_interface::Controller<hardware_interface::BlindStateInterface>
{
public:
	BlindStateController();
	bool init(hardware_interface::BlindStateInterface* hw, ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh);
	void starting(const ros::Time &time);
	void stopping(const ros::Time &time);
	void update(const ros::Time& time, const ros::Duration& period);
private:
	hardware_interface::BlindStateHandle blind_state_;
	std::shared_ptr<dls::PublisherBase<BlindStateMsgPubSubType>> pBlind_state_pub_;
	BlindStateMsg blind_state_msg_;
	uint32_t seq_;
};

PLUGINLIB_DECLARE_CLASS(ros_control_to_dls2, BlindStateController, ros_control_to_dls2::BlindStateController, controller_interface::ControllerBase);

} // namespace ros_control_to_dls2

#endif // BLIND_STATE_CONTROLLER_H
