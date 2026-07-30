
#ifndef HARDWARE_CPP
#define HARDWARE_CPP

#include "dls2/hardware/hardware.hpp"

using namespace dls;
using robotlib::RobotBase;

Hardware::Hardware
(
	const std::string& name_,
	const std::string& robot_name_
)
	: PeriodicApp(name_)
{ 
	if (robot_name_ == "")
	{
		std::string e = "Parameter robot_name is not defined, verify if the parameter server is running";
		throw std::runtime_error(e);
	}

	try
	{
		pRobot = robotlib::RobotFactory::openRobot(robot_name_);
	}
	catch (const std::exception &e)
	{
		std::cerr << "child_process: Could not open the robot " << robot_name_ << std::endl;
		std::cerr << e.what() << std::endl;
		throw;
	}
}

bool Hardware::checkActivation()
{
	hal->checkHighLevelInterface();
	hal->checkLowLevelInterface();
	return true;
}

void Hardware::run(const std::chrono::system_clock::time_point& time)
{
	//avoid unused parameter warning
	(void)time;
	hal->run();
}

#endif /* end of include guard: HARDWARE_CPP */
