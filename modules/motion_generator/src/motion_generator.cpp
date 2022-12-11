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
#include "dls2/motion_generator/motion_generator.hpp"


using namespace dls;

MotionGenerator::MotionGenerator
(
	const std::string &ID,
	const std::shared_ptr<robotlib::RobotBase>& pRobot_,
	const period_t &period_
) 
	: PeriodicAppLayerComponent(ID, period_)
	, pRobot(pRobot_)
	, ddsLink("MotionGen::" + this->getID(), dls::domains::signals)
{}

dls::DDSParticipant* MotionGenerator::getParticipant()
{
	return &(this->ddsLink);
}

const robotlib::RobotBase* MotionGenerator::getRobot()
{
	return this->pRobot.get();
}