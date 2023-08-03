
#include "dls2/msg_wrappers/attitude.hpp"

Attitude::Attitude()
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, orientation_(Eigen::Quaterniond::Identity())
	, orient_t265_(Eigen::Quaterniond::Identity())
    , angular_velocity_(Eigen::Vector3d::Zero())
	, euler_angles_t265_(Eigen::Vector3d::Zero())
	, euler_angles_estimation_(Eigen::Vector3d::Zero())
{}

Attitude::Attitude(Attitude& attitude)
    : frame_id_(attitude.frame_id_)
	, sequence_id_(attitude.sequence_id_)
	, timestamp_(attitude.timestamp_)
	, orientation_(attitude.orientation_)
	, orient_t265_(attitude.orient_t265_)
    , angular_velocity_(attitude.angular_velocity_)
	, euler_angles_t265_(attitude.euler_angles_t265_)
	, euler_angles_estimation_(attitude.euler_angles_estimation_)
{}

Attitude::~Attitude(){}

Attitude::operator AttitudeMsg() const
{
	AttitudeMsg attitude_msg;

	attitude_msg.frame_id(frame_id_);
	attitude_msg.sequence_id(sequence_id_);
	attitude_msg.timestamp(timestamp_);

	attitude_msg.orientation()[0] = orientation_.x();
	attitude_msg.orientation()[1] = orientation_.y();
	attitude_msg.orientation()[2] = orientation_.z();
	attitude_msg.orientation()[3] = orientation_.w();

	attitude_msg.orient_t265()[0] = orient_t265_.x();
	attitude_msg.orient_t265()[1] = orient_t265_.y();
	attitude_msg.orient_t265()[2] = orient_t265_.z();
	attitude_msg.orient_t265()[3] = orient_t265_.w();

	for(unsigned int i{0}; i<3; i++)
	{
    	attitude_msg.angular_velocity()[i] = angular_velocity_[i];
		attitude_msg.euler_angles_t265()[i] = euler_angles_t265_[i];
		attitude_msg.euler_angles_estimation()[i] = euler_angles_estimation_[i];
	}

	return attitude_msg;
}

Attitude& Attitude::operator=(const AttitudeMsg& attitude_msg)
{
	frame_id_ = attitude_msg.frame_id();
	sequence_id_ = attitude_msg.sequence_id();	
	timestamp_ = attitude_msg.timestamp();

	orientation_.x() = attitude_msg.orientation()[0];
	orientation_.y() = attitude_msg.orientation()[1];
	orientation_.z() = attitude_msg.orientation()[2];
	orientation_.w() = attitude_msg.orientation()[3];

	orient_t265_.x() = attitude_msg.orient_t265()[0];
	orient_t265_.y() = attitude_msg.orient_t265()[1];
	orient_t265_.z() = attitude_msg.orient_t265()[2];
	orient_t265_.w() = attitude_msg.orient_t265()[3];

	for(unsigned int i{0}; i<3; i++)
	{
    	angular_velocity_[i] = attitude_msg.angular_velocity()[i];
		euler_angles_t265_[i] = attitude_msg.euler_angles_t265()[i];
		euler_angles_estimation_[i] = attitude_msg.euler_angles_estimation()[i];
	}

    return *this;
}

Attitude& Attitude::operator=(const Attitude& attitude)
{ 
	frame_id_ = attitude.frame_id_;
	sequence_id_ = attitude.sequence_id_;
	timestamp_ = attitude.timestamp_;

    orientation_ = attitude.orientation_;
	orient_t265_ = attitude.orient_t265_;

    angular_velocity_ = attitude.angular_velocity_;
    euler_angles_t265_ = attitude.euler_angles_t265_;
	euler_angles_estimation_ = attitude.euler_angles_estimation_;
	
	return *this;
}