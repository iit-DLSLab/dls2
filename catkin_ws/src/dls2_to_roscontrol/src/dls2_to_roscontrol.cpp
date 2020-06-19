#include "dls2_to_roscontrol/dls2_to_roscontrol.hpp"

// fastrtps
//#include "topics/desired_torques.hpp"
#include "dls2/topics/control_signal_base.hpp"
#include "dls2/topics/joint_states.hpp"
#include "dls2/controller/control_signal.hpp"


// TODO remove all ROS_ERROR for new logging

#include "dls2/topics/desired_torques.hpp"

namespace dls2_to_roscontrol {

Dls2ToRoscontrol::Dls2ToRoscontrol() :
	joint_state_pub_(dls::topics::joint_states)
{
	joint_state_msg.position().resize(12);
	joint_state_msg.velocity().resize(12);
	joint_state_msg.effort().resize(12);
}

bool Dls2ToRoscontrol::init(hardware_interface::EffortJointInterface *pEffort_joint_interface, ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh)
{

	if (!pEffort_joint_interface)
	{
		ROS_ERROR("EffortJointInterface is a null pointer");
		return false;
	}
	int num_joints=12;
	char joint_names[12][13] = {"lf_haa_joint","lf_hfe_joint","lf_kfe_joint","rf_haa_joint","rf_hfe_joint","rf_kfe_joint","lh_haa_joint","lh_hfe_joint","lh_kfe_joint","rh_haa_joint","rh_hfe_joint","rh_kfe_joint"};
	for (int i = 0; i < num_joints; i++)
	{
		try {
			joint_commands_.push_back(pEffort_joint_interface->getHandle(joint_names[i]));
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
	for(size_t i = 0; i != 12; ++i)
	{
		joint_state_msg.position().push_back(joint_commands_[i].getPosition());
		joint_state_msg.velocity().push_back(joint_commands_[i].getVelocity());
		joint_state_msg.effort().push_back(joint_commands_[i].getEffort());
	}
	joint_state_pub_.publish(joint_state_msg);



	//WRITE
	std::shared_ptr<DesiredTorquesMsg> pMsg = this->control_signal_listener.getSignal();
	if(pMsg)
	{
		if (pMsg->desired_torques().size()==12)
		{
			for(size_t i = 0; i != 12; ++i) {
				joint_commands_[i].setCommand(pMsg->desired_torques()[i]);
			}
		} else {
			writeZeroTorques(time,"Desired torque vector size error.  Writing 0 torque");
		}
	}
	else
	{
		writeZeroTorques(time,"No Control signal. Writing 0 torque.");
	}
}

void Dls2ToRoscontrol::writeZeroTorques(const ros::Time& time,std::string msg) {
	if (time-previous_warning_time_>ros::Duration(30.0)) {
		std::cout << msg << std::endl;
		previous_warning_time_ = time;
	}
	for(size_t i = 0; i != 12; ++i)
	{
		joint_commands_[i].setCommand(0.0);
	}
}

void Dls2ToRoscontrol::starting(const ros::Time &time) { }
void Dls2ToRoscontrol::stopping(const ros::Time &time) { }

// =============================================================================
// Fastrtps
// =============================================================================

Dls2ToRoscontrol::ControlMsgListener::ControlMsgListener() :
	// SubscriberBase<ControlSignalMsgPubSubType>("control_signal_dls_pid_controller"),
	// pMsg(new ControlSignalMsg)
	SubscriberBase<DesiredTorquesMsgPubSubType>(dls::topics::desired_torques),
	pMsg(new DesiredTorquesMsg)
{ }

void Dls2ToRoscontrol::ControlMsgListener::onNewDataMessage(eprosima::fastrtps::Subscriber *sub)
{
	std::lock_guard<std::mutex> lock(this->msg_mutex);
	sub->takeNextData((void*)this->pMsg.get(), &info);
	// std::cout << "got signal: ";
	// for(const auto &el : this->pMsg->desired_torques())
	// {
	// 	std::cout << el << " ";
	// }
	// std::cout << std::endl;
}

std::shared_ptr<DesiredTorquesMsg> Dls2ToRoscontrol::ControlMsgListener::getSignal()
{
	std::lock_guard<std::mutex> lock(this->msg_mutex);
	return this->pMsg;
}

} //namespace dls2_to_roscontrol
