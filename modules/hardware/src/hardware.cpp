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
	, signalLink(std::make_shared<dls::DDSParticipant>(
		"Hardware::" + name_,
		dls::domains::signals
	))
{ }

std::shared_ptr<dls::DDSParticipant> Hardware::getParticipant()
{
	return this->signalLink;
}

#endif /* end of include guard: HARDWARE_CPP */