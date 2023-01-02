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

#ifndef TRUNK_CONTROLLER_DEBUG_CPP
#define TRUNK_CONTROLLER_DEBUG_CPP

#include "dls2/msg_wrappers/debug/trunk_controller_debug.hpp"

using namespace dls;

TrunkControllerDebug::TrunkControllerDebug(const std::shared_ptr<robotlib::RobotBase> pRobot)
	: feet_forces(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , desired_forces(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, desired_wrench(Eigen::Matrix<double,6,1>::Zero())
{ }

TrunkControllerDebug::TrunkControllerDebug(TrunkControllerDebug& from)
	: feet_forces(from.feet_forces)
	, desired_forces(from.desired_forces)
	, desired_wrench(from.desired_wrench)
{ }

TrunkControllerDebug::~TrunkControllerDebug()
{}

TrunkControllerDebug::operator TrunkControllerDebugMsg() const
{
    TrunkControllerDebugMsg msg;

	int i_leg = 0;
	for(auto &leg : this->feet_forces)
	{
		for(int i=0; i<3; i++)
		{
			msg.feet_forces()[i_leg*3 + i] = feet_forces[leg.key_][i];
			msg.desired_forces()[i_leg*3 + i] = desired_forces[leg.key_][i];
		}
		i_leg++;
	}

	for(int i=0;i<6;i++)
	{
		msg.desired_wrench()[i] = this->desired_wrench(i);
	}
    return msg;
}

TrunkControllerDebug& TrunkControllerDebug::operator= (const TrunkControllerDebugMsg& msg)
{
	int i_leg = 0;
	for(auto &leg : this->feet_forces)
	{
		for(int i=0; i<3; i++)
		{
			feet_forces[leg.key_][i] = msg.feet_forces()[i_leg*3 + i];
			desired_forces[leg.key_][i] = msg.desired_forces()[i_leg*3 + i];
		}
		i_leg++;
	}

	for(int i=0;i<6;i++)
	{
		this->desired_wrench(i) = msg.desired_wrench()[i];
	}
	return *this;
}

#endif // TRUNK_CONTROLLER_DEBUG_CPP