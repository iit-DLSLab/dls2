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
	xenorostask_pid(0),
	scout("hardware_layer")
{
	scout << "hello" << std::endl;
	std::cout << "ELLO" << std::endl;
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
	while(true)
	{
		int status;
		pid_t child_pid = wait(&status);
		if(child_pid == - 1 && errno == ECHILD) break;

		std::string childname;
		if(child_pid == this->xenomotor_pid)
		{
			childname = "xenomotor";
		}
		else if(child_pid == this->xenorostask_pid)
		{
			childname = "xenorostask";
		}
		if(WIFEXITED(status))
		{
			scout << childname << " exited" << std::endl;
		}
		if(WIFSIGNALED(status))
		{
			scout <<  "child process " << childname << " exited by signal" << std::endl;
		}
	// waitpid(this->xenomotor_pid, &status, 0);
	// waitpid(this->xenorostask_pid, &status, 0);
	}
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
