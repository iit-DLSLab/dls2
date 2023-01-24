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
#include "dls2/msg_wrappers/attitude.hpp"

using namespace dls;

Attitude::Attitude() 
{ }

Attitude::Attitude(Attitude& from)
    : orientation(from.orientation)
    , angular_velocity(from.angular_velocity)
	, euler_angles_imu(from.euler_angles_imu)
	, euler_angles_est(from.euler_angles_est)
	, timestamp(from.timestamp)
{ }

Attitude::operator AttitudeMsg() const
{
	AttitudeMsg msg;

	for(long unsigned int i = 0; i < msg.angular_velocity().size(); i++)
	{
    	msg.angular_velocity()[i] = this->angular_velocity[i];
		msg.euler_angles_imu()[i] = this->euler_angles_imu[i];
		msg.euler_angles_est()[i] = this->euler_angles_est[i];
	}

	msg.orientation()[0] = this->orientation.x();
	msg.orientation()[1] = this->orientation.y();
	msg.orientation()[2] = this->orientation.z();
	msg.orientation()[3] = this->orientation.w();

	msg.timestamp(this->timestamp);

	return msg;
}

Attitude& Attitude::operator=(const AttitudeMsg& msg){

	for(long unsigned int i = 0; i < msg.angular_velocity().size(); i++)
	{
    	this->angular_velocity[i] = msg.angular_velocity()[i];
		this->euler_angles_imu[i] = msg.euler_angles_imu()[i];
		this->euler_angles_est[i] = msg.euler_angles_est()[i];
	}
	
	this->orientation.x() = msg.orientation()[0];
	this->orientation.y() = msg.orientation()[1];
	this->orientation.z() = msg.orientation()[2];
	this->orientation.w() = msg.orientation()[3];

	this->timestamp = msg.timestamp();

    return *this;
}

Attitude& Attitude::operator=(const Attitude& from)
{ 
    this->orientation = from.orientation;
    this->angular_velocity = from.angular_velocity;
    this->euler_angles_imu = from.euler_angles_imu;
	this->euler_angles_est = from.euler_angles_est;
	this->timestamp = from.timestamp;
	
	return *this;
}