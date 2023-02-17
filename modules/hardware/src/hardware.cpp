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
#ifndef HARDWARE_CPP
#define HARDWARE_CPP

#include "dls2/hardware/hardware.hpp"

using namespace dls;
using robotlib::RobotBase;

Hardware::Hardware
(
	const std::string& name_,
	const std::shared_ptr<robotlib::RobotBase> &pRobot_,
	const period_t &period_
)
	: PeriodicAppLayerComponent(name_, period_)
    , pRobot(pRobot_)
	, signalLink(
		"Hardware::" + name_,
		dls::domains::signals
	)
{ }

dls::DDSParticipant* Hardware::getParticipant()
{
	return &this->signalLink;
}

bool Hardware::readBeat()
{
    bool out = this->heart_beat;
    this->heart_beat = false;

    return out;
}

#endif /* end of include guard: HARDWARE_CPP */