#include "dls2/msg_wrappers/slip_flag.hpp"

SlipFlag::SlipFlag(const std::shared_ptr<robotlib::RobotBase> robot)
	: frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, robot_name_(robot->getName())
	, slip_flag_(robot->makeLegDataMap<bool>(false))
{}

SlipFlag::SlipFlag(SlipFlag& slip_flag)
    : frame_id_(slip_flag.frame_id_)
	, sequence_id_(slip_flag.sequence_id_)
	, timestamp_(slip_flag.timestamp_)
	, robot_name_(slip_flag.robot_name_)
	, slip_flag_(slip_flag.slip_flag_)
{}

SlipFlag::~SlipFlag(){}

SlipFlag::operator SlipFlagMsg() const
{
    SlipFlagMsg slip_flag_msg;

	slip_flag_msg.frame_id(frame_id_);
	slip_flag_msg.sequence_id(sequence_id_);
	slip_flag_msg.timestamp(timestamp_);

	slip_flag_msg.robot_name() = robot_name_;

	int leg_id{0};
	for(auto &leg : slip_flag_)
	{
		slip_flag_msg.slip_flag()[leg_id] = slip_flag_[leg.key_];
		leg_id++;
	}

    return slip_flag_msg;
}

SlipFlag& SlipFlag::operator=(const SlipFlagMsg& slip_flag_msg)
{
	frame_id_ = slip_flag_msg.frame_id();
	sequence_id_ = slip_flag_msg.sequence_id();
	timestamp_ = slip_flag_msg.timestamp();

	robot_name_ = slip_flag_msg.robot_name();

	int leg_id{0};
	for(auto &leg : slip_flag_)
	{
		slip_flag_[leg.key_] = slip_flag_msg.slip_flag()[leg_id];
		leg_id++;
	}

	return *this;
}

SlipFlag& SlipFlag::operator=(const SlipFlag& slip_flag)
{
    frame_id_ = slip_flag.frame_id_;
	sequence_id_ = slip_flag.sequence_id_;
	timestamp_ = slip_flag.timestamp_;

	robot_name_ = slip_flag.robot_name_;

	slip_flag_ = slip_flag.slip_flag_;

	return *this;
}