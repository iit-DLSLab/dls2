#ifndef JOINT_STATE_CONTROLLER_H
#define JOINT_STATE_CONTROLLER_H

#include <ros/node_handle.h>
#include <hardware_interface/robot_hw.h>
#include <controller_interface/controller.h>
#include <pluginlib/class_list_macros.h>
#include <hardware_interface/joint_state_interface.h>

namespace ros_control_to_dls2 {

class JointStateController : public controller_interface::Controller<hardware_interface::JointStateInterface>
{
public:
	JointStateController();
	bool init(hardware_interface::JointStateInterface* hw, ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh);
	void starting(const ros::Time &time);
	void stopping(const ros::Time &time);
	void update(const ros::Time& time, const ros::Duration& period);
private:
	std::vector<hardware_interface::JointStateHandle> joint_states_;
};

PLUGINLIB_DECLARE_CLASS(ros_control_to_dls2, JointStateController, ros_control_to_dls2::JointStateController, controller_interface::ControllerBase);

} // namespace ros_control_to_dls2

#endif // JOINT_STATE_CONTROLLER_H
