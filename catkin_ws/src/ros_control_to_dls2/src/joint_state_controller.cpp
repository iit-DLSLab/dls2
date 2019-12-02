#include "ros_control_to_dls2/joint_state_controller.hpp"

namespace ros_control_to_dls2 {

JointStateController::JointStateController()
{
	
}

bool JointStateController::init(hardware_interface::JointStateInterface *pJoint_state_interface, ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh)
{
	if (!pJoint_state_interface)
	{
		ROS_ERROR("JointStateInterface is a null pointer");
		return false;
	}
	int num_joints=12;
	char joint_names[12][13] = {"lf_haa_joint","lf_hfe_joint","lf_kfe_joint","rf_haa_joint","rf_hfe_joint","rf_kfe_joint","lh_haa_joint","lh_hfe_joint","lh_kfe_joint","rh_haa_joint","rh_hfe_joint","rh_kfe_joint"};
	for (int i = 0; i < num_joints; i++)
	{
		joint_states_.push_back(pJoint_state_interface->getHandle(joint_names[i]));
	}
	return true;
}

void JointStateController::update(const ros::Time &time, const ros::Duration &period)
{
	for (auto js : joint_states_)
	{
		//ROS_ERROR_STREAM(js.getName() << ":" << js.getPosition());
	}
}

void JointStateController::starting(const ros::Time &time) { }
void JointStateController::stopping(const ros::Time &time) { }

} //namespace ros_control_to_dls2

