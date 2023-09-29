#include "dls2/msg_wrappers/legs_pose.hpp"

LegsPose::LegsPose(const std::shared_ptr<robotlib::RobotBase> robot)
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, linear_velocity_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, angular_velocity_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, base_velocity_(Eigen::Vector3d::Zero())
	, stance_status_(robot->makeLegDataMap<bool>(false))
{}

LegsPose::LegsPose(LegsPose& legs_pose)
    : frame_id_(legs_pose.frame_id_)
	, sequence_id_(legs_pose.sequence_id_)
	, timestamp_(legs_pose.timestamp_)
	, linear_velocity_(legs_pose.linear_velocity_)
	, angular_velocity_(legs_pose.angular_velocity_)
	, base_velocity_(legs_pose.base_velocity_)
	, stance_status_(legs_pose.stance_status_)
{}

LegsPose::~LegsPose(){}

LegsPose::operator LegsPoseMsg() const
{
    LegsPoseMsg legs_pose_msg;

	legs_pose_msg.frame_id(frame_id_);
	legs_pose_msg.sequence_id(sequence_id_);
	legs_pose_msg.timestamp(timestamp_);

	int idx{0};
	int leg_id{0};
	for(auto &leg_pair : linear_velocity_)
	{
		for(unsigned int i{0}; i<3; i++)
		{
			legs_pose_msg.linear_velocity()[idx] = linear_velocity_[leg_pair.key_](i);
			legs_pose_msg.angular_velocity()[idx] = angular_velocity_[leg_pair.key_](i);
			idx++;
		}
		legs_pose_msg.stance_status()[leg_id] = stance_status_[leg_pair.key_];
		leg_id++;
	}

	legs_pose_msg.base_velocity()[0] = base_velocity_(0);
	legs_pose_msg.base_velocity()[1] = base_velocity_(1);
	legs_pose_msg.base_velocity()[2] = base_velocity_(2);

    return legs_pose_msg;
}

LegsPose& LegsPose::operator=(const LegsPoseMsg& legs_pose_msg)
{
	frame_id_ = legs_pose_msg.frame_id();
	sequence_id_ = legs_pose_msg.sequence_id();
	timestamp_ = legs_pose_msg.timestamp();

	int idx{0};
	int leg_id{0};
	for(auto &leg_pair : linear_velocity_)
	{
		for(int i=0; i<3; i++)
		{
			linear_velocity_[leg_pair.key_](i) = legs_pose_msg.linear_velocity()[idx];
			angular_velocity_[leg_pair.key_](i) = legs_pose_msg.angular_velocity()[idx];
			idx++;
		}
		stance_status_[leg_pair.key_] = legs_pose_msg.stance_status()[leg_id];
		leg_id++;
	}

	base_velocity_[0] = legs_pose_msg.base_velocity()[0];
	base_velocity_[1] = legs_pose_msg.base_velocity()[1];
	base_velocity_[2] = legs_pose_msg.base_velocity()[2];

	return *this;
}

LegsPose& LegsPose::operator=(const LegsPose& legs_pose)
{
	frame_id_ = legs_pose.frame_id_;
	sequence_id_ = legs_pose.sequence_id_;
	timestamp_ = legs_pose.timestamp_;

	linear_velocity_ = legs_pose.linear_velocity_;
	angular_velocity_ = legs_pose.angular_velocity_;
	base_velocity_ = legs_pose.base_velocity_;

	stance_status_ = legs_pose.stance_status_;

	return *this;
}