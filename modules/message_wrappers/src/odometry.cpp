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
#include "dls2/msg_wrappers/odometry.hpp"

using namespace dls;

Odometry::Odometry() 
{ }

Odometry::Odometry(Odometry& from)
	: position(from.position)
    , orientation(from.orientation)
	, linear_velocity(from.linear_velocity)
    , angular_velocity(from.angular_velocity)
	, timestamp(from.timestamp)
{ }

Odometry::~Odometry()
{ }

Odometry::operator T265OdometryMsg() const
{
	T265OdometryMsg msg;

	for(long unsigned int i = 0; i < msg.position().size(); i++)
	{
		msg.position()[i] = this->position[i];
		msg.linear_velocity()[i] = this->linear_velocity[i];
    	msg.angular_velocity()[i] = this->angular_velocity[i];
	}

	msg.orientation()[0] = this->orientation.x();
	msg.orientation()[1] = this->orientation.y();
	msg.orientation()[2] = this->orientation.z();
	msg.orientation()[3] = this->orientation.w();

	msg.timestamp(this->timestamp);

	return msg;
}

Odometry& Odometry::operator=(T265OdometryMsg& msg){

	for(long unsigned int i = 0; i < msg.position().size(); i++)
	{
		this->position[i] = msg.position()[i];
		this->linear_velocity[i] = msg.linear_velocity()[i];
    	this->angular_velocity[i] = msg.angular_velocity()[i];
	}
	
	this->orientation.x() = msg.orientation()[0];
	this->orientation.y() = msg.orientation()[1];
	this->orientation.z() = msg.orientation()[2];
	this->orientation.w() = msg.orientation()[3];

	this->timestamp = msg.timestamp();

    return *this;
}

Odometry& Odometry::operator=(const Odometry& from)
{ 
	this->position = from.position;
    this->orientation = from.orientation;
	this->linear_velocity = from.linear_velocity;
    this->angular_velocity = from.angular_velocity;
	this->timestamp = from.timestamp;
	
	return *this;
}