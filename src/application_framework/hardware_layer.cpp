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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#include "application_framework/hardware_layer.hpp"
#include <sys/types.h>
#include <sys/wait.h>

using namespace dls;

// =============================================================================
// Constructors
// =============================================================================
HardwareLayer::HardwareLayer() :
	xenomotor_pid(0),
	xenorostask_pid(0)
{
	if((this->xenomotor_pid = fork()) == 0)
	{
		execl("./xenomotor", "xenomotor", nullptr);
	}

	if((this->xenorostask_pid = fork()) == 0)
	{
		execl("./xenorostask", "xenorostask", nullptr);
	}
}

HardwareLayer::~HardwareLayer()
{
	int status;
	waitpid(this->xenomotor_pid, &status, 0);
	waitpid(this->xenorostask_pid, &status, 0);
}


// =============================================================================
// Member Functions
// =============================================================================
HardwareLayer::Status HardwareLayer::run()
{
	return getStatus();
}

HardwareLayer::Status HardwareLayer::shutdown()
{
	return getStatus();
}

// void HardwareLayer::addSensor(std::shared_ptr<SensorBase> pSensor)
// {
// 	std::lock_guard<std::mutex> lock(this->sensors_mutex);
// 	this->sensors.push_back(pSensor);
// }

// void HardwareLayer::addSensor
// (
// 	std::initializer_list<std::shared_ptr<SensorBase>> in_sensors
// )
// {
// 	std::lock_guard<std::mutex> lock(this->sensors_mutex);
// 	this->sensors.insert(this->sensors.end(), in_sensors.begin(), in_sensors.end());
// }
