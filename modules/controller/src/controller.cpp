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
#include "dls2/controller/controller.hpp"

using namespace dls;

Controller::Controller(
	const std::string &ID_,
	const std::shared_ptr<robotlib::RobotBase> robot_,
	const ControlSignal::SignalReconstructionMethod &reconst_meth_
)
	: PeriodicAppLayerComponent(ID_)
	, signal_reconstruction_method(reconst_meth_)
	, pRobot(robot_)
	, ddsLink(std::make_shared<dls::DDSParticipant>("Controller::" + this->getID(), dls::domains::signals))
{ }
	
Controller::~Controller()
{ }

std::shared_ptr<dls::DDSParticipant> Controller::getParticipant()
{
	return this->ddsLink;
}

std::shared_ptr<robotlib::RobotBase> Controller::getRobot()
{
	return this->pRobot;
}