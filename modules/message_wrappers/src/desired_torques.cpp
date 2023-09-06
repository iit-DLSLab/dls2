

#include "dls2/msg_wrappers/desired_torques.hpp"

DesiredTorques::DesiredTorques(const std::shared_ptr<robotlib::RobotBase> robot)
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, desired_torques_(robot->makeJointState())
{}

DesiredTorques::DesiredTorques(DesiredTorques& desired_torques)
	: frame_id_(desired_torques.frame_id_)
	, sequence_id_(desired_torques.sequence_id_)
	, timestamp_(desired_torques.timestamp_)
	, desired_torques_(desired_torques.desired_torques_)
{}

DesiredTorques::~DesiredTorques(){}

DesiredTorques::operator DesiredTorquesMsg() const
{
    DesiredTorquesMsg desired_torques_msg;

	desired_torques_msg.frame_id(frame_id_);
	desired_torques_msg.sequence_id(sequence_id_);
  	desired_torques_msg.timestamp(timestamp_);

	int i{0};
	for(auto &leg : desired_torques_)
	{
		for(auto &joint : *leg.data_)
		{
			desired_torques_msg.desired_torques()[i] = desired_torques_[joint.key_]; 
			i++;
		}
	}

    return desired_torques_msg;
}

DesiredTorques& DesiredTorques::operator= (const DesiredTorquesMsg& desired_torques_msg)
{
	frame_id_ = desired_torques_msg.frame_id();
	sequence_id_ = desired_torques_msg.sequence_id();
	timestamp_ = desired_torques_msg.timestamp();

	int i{0};
	for(auto &leg : desired_torques_)
	{
		for(auto &joint : *leg.data_)
		{
			desired_torques_[joint.key_] = desired_torques_msg.desired_torques()[i];
			i++;
		}
	}

	return *this;
}

DesiredTorques& DesiredTorques::operator=(const DesiredTorques& desired_torques)
{
	frame_id_ = desired_torques.frame_id_;
	sequence_id_ = desired_torques.sequence_id_;
	timestamp_ = desired_torques.timestamp_;
	desired_torques_ = desired_torques.desired_torques_;

	return *this;
}