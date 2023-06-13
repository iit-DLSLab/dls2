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

#ifndef SLIP_FLAG_CPP
#define SLIP_FLAG_CPP

#include "dls2/msg_wrappers/slip_flag.hpp"

using namespace dls;

SlipFlag::SlipFlag(const std::shared_ptr<robotlib::RobotBase> pRobot)
	: robot_name("")
	, slip_flag(pRobot->makeLegDataMap<bool>(false))
	, time(0)
{ }

SlipFlag::SlipFlag(SlipFlag& from)
	: robot_name(from.robot_name)
	, slip_flag(from.slip_flag)
	, time(from.time)
{ }

SlipFlag::~SlipFlag()
{ }

SlipFlag::operator SlipFlagMsg() const
{
    SlipFlagMsg msg;

	msg.robot_name() = this->robot_name;

	int leg_id = 0;
	for(auto &leg : this->slip_flag)
	{
		msg.slip_flag()[leg_id] = this->slip_flag[leg.key_];
		leg_id++;
	}

	msg.time(this->time);

    return msg;
}

SlipFlag& SlipFlag::operator= (const SlipFlagMsg& msg)
{
	this->robot_name = msg.robot_name();

	int leg_id = 0;
	for(auto &leg : this->slip_flag)
	{
		this->slip_flag[leg.key_] = msg.slip_flag()[leg_id];
		leg_id++;
	}

	this->time = msg.time();

	return *this;
}

#endif // SLIP_FLAG_CPP