

#ifndef STANCE_LEGS_CPP
#define STANCE_LEGS_CPP

#include "dls2/msg_wrappers/stance_legs.hpp"

using namespace dls;

StanceLegs::StanceLegs(const std::shared_ptr<robotlib::RobotBase> pRobot)
	: robot_name("")
	, stance_legs(pRobot->makeLegDataMap<bool>(false))
	, time(0)
{ }

StanceLegs::StanceLegs(StanceLegs& from)
	: robot_name(from.robot_name)
	, stance_legs(from.stance_legs)
	, time(from.time)
{ }

StanceLegs::~StanceLegs()
{ }

StanceLegs::operator StanceLegsMsg() const
{
    StanceLegsMsg msg;

	msg.robot_name() = this->robot_name;

	int leg_id = 0;
	for(auto &leg : this->stance_legs)
	{
		msg.stance_legs()[leg_id] = this->stance_legs[leg.key_];
		leg_id++;
	}

	msg.time(this->time);

    return msg;
}

StanceLegs& StanceLegs::operator= (const StanceLegsMsg& msg)
{
	this->robot_name = msg.robot_name();

	int leg_id = 0;
	for(auto &leg : this->stance_legs)
	{
		this->stance_legs[leg.key_] = msg.stance_legs()[leg_id];
		leg_id++;
	}

	this->time = msg.time();

	return *this;
}

#endif // STANCE_LEGS_CPP