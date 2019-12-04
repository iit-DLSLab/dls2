#include "ros_control_to_dls2/blind_state_controller.hpp"

namespace ros_control_to_dls2 {

BlindStateController::BlindStateController()
{
	
}

bool BlindStateController::init(hardware_interface::BlindStateInterface *pBlind_state_interface, ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh)
{
	if (!pBlind_state_interface)
	{
		ROS_ERROR("BlindStateInterface is a null pointer");
		return false;
	}
	
	pBlind_state_pub_ = std::make_shared<dls::PublisherBase<BlindStateMsgPubSubType>> (dls::topics::low_level_estimation::blind_state);
	seq_=0;
	blind_state_msg_.joint_state().position().resize(12);
	blind_state_msg_.joint_state().velocity().resize(12);
	blind_state_msg_.joint_state().acceleration().resize(12);
	blind_state_msg_.joint_state().effort().resize(12);
	blind_state_ = pBlind_state_interface->getHandle("blind_state");
	return true;
}

void BlindStateController::update(const ros::Time &time, const ros::Duration &period)
{
	blind_state_msg_.header().time().seconds() = double(time.sec)+double(time.nsec)/1e9;
	blind_state_msg_.header().seq() = seq_++;
	for (int i=0;i<12;i++)
	{
		blind_state_msg_.joint_state().position()[i] = blind_state_.getJointPosition()[i];
		blind_state_msg_.joint_state().velocity()[i] = blind_state_.getJointVelocity()[i];;
		blind_state_msg_.joint_state().acceleration()[i] = blind_state_.getJointAcceleration()[i];
		blind_state_msg_.joint_state().effort()[i] = blind_state_.getJointEffort()[i];
	}
	for (int i=0;i<3;i++)
	{
		blind_state_msg_.base_pose_world().position()[i] = blind_state_.getBasePoseWorld()[i];
		blind_state_msg_.base_velocity_world().linear()[i] = blind_state_.getBaseVelocityWorld()[i];
		blind_state_msg_.base_velocity_world().angular()[i] = blind_state_.getBaseVelocityWorld()[i+3];
		blind_state_msg_.base_acceleration_world().linear()[i] = blind_state_.getBaseAccelerationWorld()[i];
		blind_state_msg_.base_acceleration_world().angular()[i] = blind_state_.getBaseAccelerationWorld()[i+3];
	}
	for  (int i=0;i<4;i++)
	{
		blind_state_msg_.base_pose_world().quaternion()[i] = blind_state_.getBasePoseWorld()[i+3];
	}
	pBlind_state_pub_->publish(blind_state_msg_);
}

void BlindStateController::starting(const ros::Time &time) { }
void BlindStateController::stopping(const ros::Time &time) { }

} //namespace ros_control_to_dls2


