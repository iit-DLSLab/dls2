
#include "dls2/msg_wrappers/t265_odometry.hpp"

T265Odometry::T265Odometry()
	: frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, position_(Eigen::Vector3d::Zero())
    , orientation_(Eigen::Quaterniond::Identity())
	, orientation_rpy_(Eigen::Vector3d::Zero())
	, linear_velocity_(Eigen::Vector3d::Zero())
    , angular_velocity_(Eigen::Vector3d::Zero())
{}

T265Odometry::T265Odometry(T265Odometry& t265_odometry)
    : frame_id_(t265_odometry.frame_id_)
	, sequence_id_(t265_odometry.sequence_id_)
	, timestamp_(t265_odometry.timestamp_)
	, position_(t265_odometry.position_)
    , orientation_(t265_odometry.orientation_)
	, orientation_rpy_(t265_odometry.orientation_rpy_)
	, linear_velocity_(t265_odometry.linear_velocity_)
    , angular_velocity_(t265_odometry.angular_velocity_)
{}

T265Odometry::~T265Odometry(){}

T265Odometry::operator T265OdometryMsg() const
{
	T265OdometryMsg t265_odometry_msg;

	t265_odometry_msg.frame_id(frame_id_);
	t265_odometry_msg.sequence_id(sequence_id_);
	t265_odometry_msg.timestamp(timestamp_);

	for(unsigned int i{0}; i<3; i++)
	{
		t265_odometry_msg.position()[i] = position_[i];
		t265_odometry_msg.linear_velocity()[i] = linear_velocity_[i];
    	t265_odometry_msg.angular_velocity()[i] = angular_velocity_[i];
	}

	t265_odometry_msg.orientation()[0] = orientation_.x();
	t265_odometry_msg.orientation()[1] = orientation_.y();
	t265_odometry_msg.orientation()[2] = orientation_.z();
	t265_odometry_msg.orientation()[3] = orientation_.w();
	t265_odometry_msg.orientation_rpy()[0] = orientation_rpy_(0);
	t265_odometry_msg.orientation_rpy()[1] = orientation_rpy_(1);
	t265_odometry_msg.orientation_rpy()[2] = orientation_rpy_(2);

	return t265_odometry_msg;
}

T265Odometry& T265Odometry::operator=(const T265OdometryMsg& t265_odometry_msg)
{
	frame_id_ = t265_odometry_msg.frame_id();
	sequence_id_ = t265_odometry_msg.sequence_id();
	timestamp_ = t265_odometry_msg.timestamp();

	for(unsigned int i{0}; i<3; i++)
	{
		position_[i] = t265_odometry_msg.position()[i];
		linear_velocity_[i] = t265_odometry_msg.linear_velocity()[i];
    	angular_velocity_[i] = t265_odometry_msg.angular_velocity()[i];
	}
	
	orientation_.x() = t265_odometry_msg.orientation()[0];
	orientation_.y() = t265_odometry_msg.orientation()[1];
	orientation_.z() = t265_odometry_msg.orientation()[2];
	orientation_.w() = t265_odometry_msg.orientation()[3];
	orientation_rpy_(0) = t265_odometry_msg.orientation_rpy()[0];
	orientation_rpy_(1) = t265_odometry_msg.orientation_rpy()[1];
	orientation_rpy_(2) = t265_odometry_msg.orientation_rpy()[2];

    return *this;
}

T265Odometry& T265Odometry::operator=(const T265Odometry& t265_odometry)
{
	frame_id_ = t265_odometry.frame_id_;
	sequence_id_ = t265_odometry.sequence_id_;
	timestamp_ = t265_odometry.timestamp_;

	position_ = t265_odometry.position_;
    orientation_ = t265_odometry.orientation_;
    orientation_rpy_ = t265_odometry.orientation_rpy_;

	linear_velocity_ = t265_odometry.linear_velocity_;
    angular_velocity_ = t265_odometry.angular_velocity_;
	
	return *this;
}

void T265Odometry::setDataFromWrapperBase(WrapperBase* wrapper_base)
{
    // Assuming wrapper is of type Wrapper<MsgType>
    *this = static_cast<T265Odometry&>(*wrapper_base);
}