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

#ifndef BASE_STATE_CPP
#define BASE_STATE_CPP

#include "dls2/msg_wrappers/base_state.hpp"

using namespace dls;

BaseState::BaseState()
	: robot_name("")
	, time(0)
{ }

BaseState::BaseState(BaseState& from)
	: robot_name(from.robot_name)
	, pose(from.pose)
	, velocity(from.velocity)
	, acceleration(from.acceleration)
	, time(from.time)
{ }

BaseState::~BaseState()
{ }

BaseState::operator BaseStateMsg() const
{
    BaseStateMsg msg;

	msg.robot_name() = this->robot_name;

	for(int i=0; i<3;i++)
	{
		msg.position()[i] = this->pose.toPosition()[i];
		msg.linear_velocity()[i] = this->velocity.getLinear()[i];
		msg.angular_velocity()[i] = this->velocity.getAngular()[i];
		msg.linear_acceleration()[i] = this->acceleration.getLinear()[i];
		msg.angular_acceleration()[i] = this->acceleration.getAngular()[i];
	}

	msg.orientation()[0] = this->pose.toQuaternion().x();
	msg.orientation()[1] = this->pose.toQuaternion().y();
	msg.orientation()[2] = this->pose.toQuaternion().z();
	msg.orientation()[3] = this->pose.toQuaternion().w();

	msg.time(this->time);

    return msg;
}

BaseState& BaseState::operator= (const BaseStateMsg& msg)
{
	this->robot_name = msg.robot_name();

	this->pose.set(Eigen::Vector3d(msg.position().data()));
	this->pose.set(Eigen::Quaterniond(msg.orientation()[3], msg.orientation()[0], msg.orientation()[1], msg.orientation()[2]));
	this->velocity.setLinear(Eigen::Vector3d(msg.linear_velocity().data()));
	this->velocity.setAngular(Eigen::Vector3d(msg.angular_velocity().data()));
	this->acceleration.setLinear(Eigen::Vector3d(msg.linear_acceleration().data()));
	this->acceleration.setAngular(Eigen::Vector3d(msg.angular_acceleration().data()));

	this->time = msg.time();

	return *this;
}

#endif // BASE_STATE_CPP