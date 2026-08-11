
#ifndef HARDWARE_CPP
#define HARDWARE_CPP

#include "dls2/hardware/hardware.hpp"

using namespace dls;
using robotlib::RobotBase;

Hardware::Hardware
(
	const std::string& name_
)
	: PeriodicApp(name_)
{}

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
