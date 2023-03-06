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

#ifndef LEGS_POSE_CPP
#define LEGS_POSE_CPP

#include "dls2/msg_wrappers/legs_pose.hpp"

using namespace dls;

LegsPose::LegsPose(const std::shared_ptr<robotlib::RobotBase>& pRobot)
	: lin_velocity(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, ang_velocity(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, time(0.0)
{}

LegsPose::LegsPose(LegsPose& from)
	: lin_velocity(from.lin_velocity)
	, ang_velocity(from.ang_velocity)
	, base_velocity(from.base_velocity)
	, time(from.time)
{}

LegsPose::~LegsPose()
{}

LegsPose::operator LegsPoseMsg() const
{
    LegsPoseMsg msg;

	int idx = 0;
	for(auto &leg_pair : this->lin_velocity)
	{
		for(int i=0; i<3; i++)
		{
			msg.lin_velocity()[idx] = this->lin_velocity[leg_pair.key_](i);
			msg.ang_velocity()[idx] = this->ang_velocity[leg_pair.key_](i);
			idx++;
		}
	}

	msg.base_velocity()[0] = this->base_velocity(0);
	msg.base_velocity()[1] = this->base_velocity(1);
	msg.base_velocity()[2] = this->base_velocity(2);

  	msg.time(this->time);

    return msg;
}

LegsPose& LegsPose::operator= (const LegsPoseMsg& msg)
{
	int idx = 0;
	for(auto &leg_pair : this->lin_velocity)
	{
		for(int i=0; i<3; i++)
		{
			this->lin_velocity[leg_pair.key_](i) = msg.lin_velocity()[idx];
			this->ang_velocity[leg_pair.key_](i) = msg.ang_velocity()[idx];
			idx++;
		}
	}

	this->base_velocity[0] = msg.base_velocity()[0];
	this->base_velocity[1] = msg.base_velocity()[1];
	this->base_velocity[2] = msg.base_velocity()[2];

	this->time = msg.time();

	return *this;
}

#endif // LEGS_POSE_CPP