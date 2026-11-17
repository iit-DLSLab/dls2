#include "dls2/controller/controller.hpp"

using namespace dls;

Controller::Controller(
	const std::string &ID,
	const std::shared_ptr<robotlib::RobotBase> robot_
)
	: PeriodicApp(ID)
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