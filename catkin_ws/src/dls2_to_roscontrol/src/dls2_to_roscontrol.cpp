#include "dls2_to_roscontrol/dls2_to_roscontrol.hpp"

namespace dls2_to_roscontrol {

Dls2ToRoscontrol::Dls2ToRoscontrol()
{
	
}

bool Dls2ToRoscontrol::init(hardware_interface::JointCommandAdvInterface *pJoint_command_adv_interface, ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh)
{
	if (!pJoint_command_adv_interface)
	{
		ROS_ERROR("JointCommandAdvInterface is a null pointer");
		return false;
	}
	int num_joints=12;
	char joint_names[12][13] = {"lf_haa_joint","lf_hfe_joint","lf_kfe_joint","rf_haa_joint","rf_hfe_joint","rf_kfe_joint","lh_haa_joint","lh_hfe_joint","lh_kfe_joint","rh_haa_joint","rh_hfe_joint","rh_kfe_joint"};
	for (int i = 0; i < num_joints; i++)
	{
		joint_commands_.push_back(pJoint_command_adv_interface->getHandle(joint_names[i]));
	}
	return true;
}

void Dls2ToRoscontrol::update(const ros::Time &time, const ros::Duration &period)
{
	for (auto jc : joint_commands_)
	{
		jc.setCommand(0,0,0);
		jc.setCommandGains(1,1,1);
	}
}

void Dls2ToRoscontrol::starting(const ros::Time &time) { }
void Dls2ToRoscontrol::stopping(const ros::Time &time) { }

} //namespace dls2_to_roscontrol

