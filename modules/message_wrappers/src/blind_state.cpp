#include "dls2/msg_wrappers/blind_state.hpp"

BlindState::BlindState(const std::shared_ptr<robotlib::RobotBase> robot)
	: frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, robot_name_(robot->getName())
	, joints_name_(robot->makeJointDataMap<std::string>(""))
	, joints_position_(robot->makeJointState())
	, joints_velocity_(robot->makeJointState())
	, joints_acceleration_(robot->makeJointState())
	, joints_effort_(robot->makeJointState())
	, joints_temperature_(robot->makeJointState())
	, feet_contact_(robot->makeLegDataMap<bool>(false))
{}

BlindState::BlindState(BlindState& blind_state)
	: frame_id_(blind_state.frame_id_)
	, sequence_id_(blind_state.sequence_id_)
	, timestamp_(blind_state.timestamp_)
	, robot_name_(blind_state.robot_name_)
	, joints_name_(blind_state.joints_name_) 
	, joints_position_(blind_state.joints_position_)
	, joints_velocity_(blind_state.joints_velocity_)
	, joints_acceleration_(blind_state.joints_acceleration_)
	, joints_effort_(blind_state.joints_effort_)
	, joints_temperature_(blind_state.joints_temperature_)
	, feet_contact_(blind_state.feet_contact_)
{}

BlindState::~BlindState(){}

BlindState::operator BlindStateMsg() const
{
    BlindStateMsg blind_state_msg;

	blind_state_msg.frame_id(frame_id_);
	blind_state_msg.sequence_id(sequence_id_);
	blind_state_msg.timestamp(timestamp_);

	blind_state_msg.robot_name(robot_name_);

	int leg_id{0};
	int leg_joint_id{0};

	for(auto &leg : joints_position_)
	{
		leg_joint_id = 0;
		int idx = leg_id*leg.key_->getNJoints();
		for(auto &joint : *leg.data_)
		{
			blind_state_msg.joints_name()[idx+leg_joint_id] = joints_name_[joint.key_];
			blind_state_msg.joints_position()[idx+leg_joint_id] = joints_position_[joint.key_];
			blind_state_msg.joints_velocity()[idx+leg_joint_id] = joints_velocity_[joint.key_];
			blind_state_msg.joints_acceleration()[idx+leg_joint_id] = joints_acceleration_[joint.key_];
			blind_state_msg.joints_effort()[idx+leg_joint_id] = joints_effort_[joint.key_];
			blind_state_msg.joints_temperature()[idx+leg_joint_id] = joints_temperature_[joint.key_];
			leg_joint_id++;
		}

		blind_state_msg.feet_contact()[leg_id] = feet_contact_[leg.key_];
		leg_id++;
	}

    return blind_state_msg;
}

BlindState& BlindState::operator=(const BlindStateMsg& blind_state_msg)
{
	frame_id_ = blind_state_msg.frame_id();
	sequence_id_ = blind_state_msg.sequence_id();
	timestamp_ = blind_state_msg.timestamp();

	robot_name_ = blind_state_msg.robot_name();

	int leg_id{0};
	for(auto &leg : joints_position_)
	{
		int i = leg_id*leg.key_->getNJoints();

		for(auto &joint : *leg.data_)
		{
			joints_name_[joint.key_] = blind_state_msg.joints_name()[i];
			joints_position_[joint.key_] = blind_state_msg.joints_position()[i];
			joints_velocity_[joint.key_] = blind_state_msg.joints_velocity()[i];
			joints_acceleration_[joint.key_] = blind_state_msg.joints_acceleration()[i];
			joints_effort_[joint.key_] = blind_state_msg.joints_effort()[i];
			joints_temperature_[joint.key_] = blind_state_msg.joints_temperature()[i];
			i++;
		}

		feet_contact_[leg.key_] = blind_state_msg.feet_contact()[leg_id];
		leg_id++;
	}

	return *this;
}

BlindState& BlindState::operator=(const BlindState& blind_state)
{
	frame_id_ = blind_state.frame_id_;
	sequence_id_ = blind_state.sequence_id_;
	timestamp_ = blind_state.timestamp_;

	robot_name_ = blind_state.robot_name_;

	joints_name_ = blind_state.joints_name_;
	joints_position_ = blind_state.joints_position_;
	joints_velocity_ = blind_state.joints_velocity_;
	joints_acceleration_ = blind_state.joints_acceleration_;
	joints_effort_ = blind_state.joints_effort_;
	joints_temperature_ = blind_state.joints_temperature_;

	feet_contact_ = blind_state.feet_contact_;

	return *this;
}