
#include "dls2/msg_wrappers/imu.hpp"

Imu::Imu()
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, orientation_(Eigen::Quaterniond::Identity())
	, orientation_covariance_(Eigen::Matrix3d::Zero())
	, angular_velocity_(Eigen::Vector3d::Zero())
	, angular_velocity_covariance_(Eigen::Matrix3d::Zero())
	, linear_acceleration_(Eigen::Vector3d::Zero())
	, linear_acceleration_covariance_(Eigen::Matrix3d::Zero())
{}

Imu::Imu(Imu& imu)
    : frame_id_(imu.frame_id_)
	, sequence_id_(imu.sequence_id_)
	, timestamp_(imu.timestamp_)
	, orientation_(imu.orientation_)
	, orientation_covariance_(imu.orientation_covariance_)
	, angular_velocity_(imu.angular_velocity_)
	, angular_velocity_covariance_(imu.angular_velocity_covariance_)
	, linear_acceleration_(imu.linear_acceleration_)
	, linear_acceleration_covariance_(imu.linear_acceleration_covariance_)
{}

Imu::~Imu(){}

Imu::operator ImuMsg() const
{
	ImuMsg imu_msg;

	imu_msg.frame_id(frame_id_);
	imu_msg.sequence_id(sequence_id_);
	imu_msg.timestamp(timestamp_);

	imu_msg.orientation()[0] = orientation_.x();
	imu_msg.orientation()[1] = orientation_.y();
	imu_msg.orientation()[2] = orientation_.z();
	imu_msg.orientation()[3] = orientation_.w();

	for(unsigned int i{0}; i<3; i++)
	{
		imu_msg.angular_velocity()[i] = angular_velocity_[i];
		imu_msg.linear_acceleration()[i] = linear_acceleration_[i];

		for(unsigned int j{0}; j<3; j++)
		{
			imu_msg.orientation_covariance()[i*3+j] = orientation_covariance_(i,j);
			imu_msg.angular_velocity_covariance()[i*3+j] = angular_velocity_covariance_(i,j);
			imu_msg.linear_acceleration_covariance()[i*3+j] = linear_acceleration_covariance_(i,j);
		}
	}

    return imu_msg;
}

Imu& Imu::operator= (const ImuMsg& imu_msg)
{
	frame_id_ = imu_msg.frame_id();
	sequence_id_ = imu_msg.sequence_id();
	timestamp_ = imu_msg.timestamp();

	orientation_.x() = imu_msg.orientation()[0];
	orientation_.y() = imu_msg.orientation()[1];
	orientation_.z() = imu_msg.orientation()[2];
	orientation_.w() = imu_msg.orientation()[3];

	for(unsigned int i{0}; i<3; i++)
	{
		angular_velocity_(i) = imu_msg.angular_velocity()[i];
		linear_acceleration_(i) = imu_msg.linear_acceleration()[i];

		for(unsigned int j{0}; j<3; j++)
		{
			orientation_covariance_(i,j) = imu_msg.orientation_covariance()[i*3+j];
			angular_velocity_covariance_(i,j) = imu_msg.angular_velocity_covariance()[i*3+j];
			linear_acceleration_covariance_(i,j) = imu_msg.linear_acceleration_covariance()[i*3+j];
		}
	}

	return *this;
}

Imu& Imu::operator=(const Imu& imu)
{
    frame_id_ = imu.frame_id_;
	sequence_id_ = imu.sequence_id_;
	timestamp_ = imu.timestamp_;

	orientation_ = imu.orientation_;
	orientation_covariance_ = imu.orientation_covariance_;
	angular_velocity_ = imu.angular_velocity_;
	angular_velocity_covariance_ = imu.angular_velocity_covariance_;
	linear_acceleration_ = imu.linear_acceleration_;
	linear_acceleration_covariance_ = imu.linear_acceleration_covariance_;

	return *this;
}

void Imu::setDataFromWrapperBase(WrapperBase* wrapper_base)
{
    // Assuming wrapper is of type Wrapper<MsgType>
    *this = static_cast<Imu&>(*wrapper_base);
}