#include "dls2/msg_wrappers/stance_status.hpp"

StanceStatus::StanceStatus(const std::shared_ptr<robotlib::RobotBase> robot)
	: frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, stance_status_(robot->makeLegDataMap<bool>(false))
{}

StanceStatus::StanceStatus(StanceStatus& stance_status)
	: frame_id_(stance_status.frame_id_)
	, sequence_id_(stance_status.sequence_id_)
	, timestamp_(stance_status.timestamp_)
	, stance_status_(stance_status.stance_status_)
{}

StanceStatus::~StanceStatus(){}

StanceStatus::operator StanceStatusMsg() const
{
    StanceStatusMsg stance_status_msg;

	stance_status_msg.frame_id(frame_id_);
	stance_status_msg.sequence_id(sequence_id_);
	stance_status_msg.timestamp(timestamp_);

	int leg_id{0};

	for(auto &leg : stance_status_)
	{
		stance_status_msg.stance_status()[leg_id] = stance_status_[leg.key_];
		leg_id++;
	}

    return stance_status_msg;
}

StanceStatus& StanceStatus::operator=(const StanceStatusMsg& stance_status_msg)
{
	frame_id_ = stance_status_msg.frame_id();
	sequence_id_ = stance_status_msg.sequence_id();
	timestamp_ = stance_status_msg.timestamp();

	int leg_id{0};
	for(auto &leg : stance_status_)
	{
		stance_status_[leg.key_] = stance_status_msg.stance_status()[leg_id];
		leg_id++;
	}

	return *this;
}

StanceStatus& StanceStatus::operator=(const StanceStatus& stance_status)
{
	frame_id_ = stance_status.frame_id_;
	sequence_id_ = stance_status.sequence_id_;
	timestamp_ = stance_status.timestamp_;

	stance_status_ = stance_status.stance_status_;

	return *this;
}