#include "dls2_to_roscontrol/dls2_to_roscontrol.hpp"

// fastrtps
//#include "topics/desired_torques.hpp"
#include "topics/control_signal_base.hpp"
#include "topics/joint_states.hpp"
#include "controller/control_signal.hpp"

// TODO remove all ROS_ERROR for new logging

namespace dls2_to_roscontrol {

Dls2ToRoscontrol::Dls2ToRoscontrol() :
	joint_state_pub_(topics::joint_states)
{ }

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
		try {
			joint_commands_.push_back(pJoint_command_adv_interface->getHandle(joint_names[i]));
		} catch (hardware_interface::HardwareInterfaceException ex)
		{
			ROS_ERROR_STREAM("ERROR: " << joint_names[i] << " does not exist.");
		} catch (...)
		{
			ROS_ERROR("UNKOWN ERROR");
		}

	}
	return true;
}

void Dls2ToRoscontrol::update(const ros::Time &time, const ros::Duration &period)
{
	//READ
	JointStateMsg joint_state_msg;
	joint_state_msg.position().resize(12);
	joint_state_msg.velocity().resize(12);
	joint_state_msg.effort().resize(12);

	for(size_t i = 0; i != 12; ++i)
	{
		joint_state_msg.position().push_back(joint_commands_[i].getPosition());
		joint_state_msg.velocity().push_back(joint_commands_[i].getVelocity());
		joint_state_msg.effort().push_back(joint_commands_[i].getEffort());
	}
	joint_state_pub_.publish(joint_state_msg);


	//WRITE
	if(auto pMsg = this->control_signal_listener.getSignal())
	{
		ControlSignal s; // *pMsg
		// message received from framework
		// for (auto jc : joint_commands_)
		//std::vector<double> vec = pMsg->desired_torques();
		
		if (pMsg->torques().size()!=12)
		{
			std::cout << "Desired torque vector size error (" << pMsg->torques().size() << ").  Writing 0 torque" << std::endl;
			for(size_t i = 0; i != 12; ++i)
			{
				joint_commands_[i].setCommandEffort(0.0);
			}
		}
		else
		{
			for(size_t i = 0; i != 12; ++i)
			{
				joint_commands_[i].setCommandPosition(0.0);
				joint_commands_[i].setCommandVelocity(0.0);
				joint_commands_[i].setCommandEffort(pMsg->torques()[i]);
				joint_commands_[i].setCommandGains(0.0,0.0,0.0);
			}
		}
	}
	else
	{
		std::cout << "No Control signal. Writing 0 torque." << std::endl;
		for(size_t i = 0; i != 12; ++i)
		{
			joint_commands_[i].setCommandEffort(0.0);
		}
	}
}

void Dls2ToRoscontrol::starting(const ros::Time &time) { }
void Dls2ToRoscontrol::stopping(const ros::Time &time) { }

// =============================================================================
// Fastrtps
// =============================================================================

Dls2ToRoscontrol::ControlMsgListener::ControlMsgListener() :
	SubscriberBase<ControlSignalMsgPubSubType>("control_signal_pid_controller"),
	pMsg(new ControlSignalMsg)
{ }

void Dls2ToRoscontrol::ControlMsgListener::onNewDataMessage(eprosima::fastrtps::Subscriber *sub)
{
	std::lock_guard<std::mutex> lock(this->msg_mutex);
	sub->takeNextData((void*)this->pMsg.get(), &info);
}

std::shared_ptr<ControlSignalMsg> Dls2ToRoscontrol::ControlMsgListener::getSignal()
{
	std::lock_guard<std::mutex> lock(this->msg_mutex);
	return this->pMsg;
}

} //namespace dls2_to_roscontrol
