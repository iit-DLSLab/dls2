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

#ifndef DESIRED_TORQUES_CPP
#define DESIRED_TORQUES_CPP

#include "dls2/msg_wrappers/desired_torques.hpp"

using namespace dls;

DesiredTorques::DesiredTorques(const std::shared_ptr<robotlib::RobotBase> pRobot)
	: desired_torques(pRobot->makeJointState())
	, time(0.0)
	, seq(0)
	, frame_id(pRobot->getName())
{}

DesiredTorques::DesiredTorques(DesiredTorques& from)
	: desired_torques(from.desired_torques)
	, time(from.time)
	, seq(from.seq)
	, frame_id(from.frame_id)
{}

DesiredTorques::~DesiredTorques()
{}

DesiredTorques::operator DesiredTorquesMsg() const
{
    DesiredTorquesMsg msg;

	int i = 0;
	for(auto &leg : this->desired_torques)
	{
		for(auto &joint : *leg.data_)
		{
			msg.desired_torques()[i] = this->desired_torques[joint.key_]; 
			i++;
		}
	}

  	msg.time(this->time);
	msg.seq(this->seq);
	msg.frame_id(this->frame_id);

    return msg;
}

DesiredTorques& DesiredTorques::operator= (DesiredTorquesMsg& msg)
{
	int i = 0;
	for(auto &leg : this->desired_torques)
	{
		for(auto &joint : *leg.data_)
		{
			this->desired_torques[joint.key_] = msg.desired_torques()[i];
			i++;
		}
	}

	this->time = msg.time();
	this->seq = msg.seq();
	this->frame_id = msg.frame_id();

	return *this;
}

dls::DesiredTorques& dls::DesiredTorques::operator=(const dls::DesiredTorques& from)
{
	for(auto &leg : this->desired_torques)
	{
		for(auto &joint : *leg.data_)
		{
			this->desired_torques[joint.key_] = from.desired_torques[joint.key_];
		}
	}

	this->time = from.time;
	this->seq = from.seq;
	this->frame_id = from.frame_id;

	return *this;
}
#endif // DESIRED_TORQUES_CPP