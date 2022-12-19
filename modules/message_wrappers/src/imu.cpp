/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/

#ifndef IMU_CPP
#define IMU_CPP

#include "dls2/msg_wrappers/imu.hpp"

using namespace dls;

Imu::Imu()
	: orientation(0.0, 0.0, 0.0, 1.0)
	, orientation_covariance(Eigen::Matrix3d::Zero())
	, angular_velocity(Eigen::Vector3d::Zero())
	, angular_velocity_covariance(Eigen::Matrix3d::Zero())
	, linear_acceleration(Eigen::Vector3d::Zero())
	, linear_acceleration_covariance(Eigen::Matrix3d::Zero())
	, time(0.0)
	, frame_id("")
{}

Imu::Imu(Imu& from)
	: orientation(0.0, 0.0, 0.0, 1.0)
	, orientation_covariance(Eigen::Matrix3d::Zero())
	, angular_velocity(Eigen::Vector3d::Zero())
	, angular_velocity_covariance(Eigen::Matrix3d::Zero())
	, linear_acceleration(Eigen::Vector3d::Zero())
	, linear_acceleration_covariance(Eigen::Matrix3d::Zero())
	, time(from.time)
	, frame_id(from.frame_id)
{}

Imu::~Imu()
{}

Imu::operator ImuMsg() const
{
	ImuMsg msg;

	msg.orientation()[0] = this->orientation.x();
	msg.orientation()[1] = this->orientation.y();
	msg.orientation()[2] = this->orientation.z();
	msg.orientation()[3] = this->orientation.w();

	for(int i=0; i<3; i++)
	{
		msg.angular_velocity()[i] = this->angular_velocity[i];
		msg.linear_acceleration()[i] = this->linear_acceleration[i];
		for(int j=0; j<3; j++)
		{
			msg.orientation_covariance()[i*3+j] = this->orientation_covariance(i,j);
			msg.angular_velocity_covariance()[i*3+j] = this->angular_velocity_covariance(i,j);
			msg.linear_acceleration_covariance()[i*3+j] = this->linear_acceleration_covariance(i,j);
		}
	}

	msg.time(this->time);
	msg.frame_id(this->frame_id);

    return msg;
}

Imu& Imu::operator= (ImuMsg& msg)
{
	this->orientation.x() = msg.orientation()[0];
	this->orientation.y() = msg.orientation()[1];
	this->orientation.z() = msg.orientation()[2];
	this->orientation.w() = msg.orientation()[3];

	for(int i=0; i<3; i++)
	{
		this->angular_velocity(i) = msg.angular_velocity()[i];
		this->linear_acceleration(i) = msg.linear_acceleration()[i];
		for(int j=0; j<3; j++)
		{
			this->orientation_covariance(i,j) = msg.orientation_covariance()[i*3+j];
			this->angular_velocity_covariance(i,j) = msg.angular_velocity_covariance()[i*3+j];
			this->linear_acceleration_covariance(i,j) = msg.linear_acceleration_covariance()[i*3+j];
		}
	}

	this->time = msg.time();
	this->frame_id = msg.frame_id();

	return *this;
}

#endif // IMU_CPP