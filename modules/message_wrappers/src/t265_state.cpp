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
#include "dls2/msg_wrappers/t265_state.hpp"

using namespace dls;

T265State::T265State()
	// : robot_name("")
	// , time(0)
{ }

T265State::T265State(T265State& from)
	: robot_name(from.robot_name)
	, position(from.position)
    , orientation(from.orientation)
	, linear_velocity(from.linear_velocity)
    , angular_velocity(from.angular_velocity)
	, timestamp(from.timestamp)
{ }

T265State::~T265State()
{ }

T265State::operator T265StateMsg() const
{
    T265StateMsg msg;

	msg.robot_name() = this->robot_name;

	for(int i=0; i<3;i++)
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

T265State& T265State::operator= (const T265StateMsg& msg)
{
	this->robot_name = msg.robot_name();

	// this->pose.set(Eigen::Vector3d(msg.position().data()));
	// this->pose.set(Eigen::Quaterniond(msg.orientation()[3], msg.orientation()[0], msg.orientation()[1], msg.orientation()[2]));
	// this->velocity.setLinear(Eigen::Vector3d(msg.linear_velocity().data()));
	// this->velocity.setAngular(Eigen::Vector3d(msg.angular_velocity().data()));

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

T265State& T265State::operator=(const T265State& from)
{ 
	this->position = from.position;
    this->orientation = from.orientation;
	this->linear_velocity = from.linear_velocity;
    this->angular_velocity = from.angular_velocity;
	this->timestamp = from.timestamp;
	
	return *this;
}

