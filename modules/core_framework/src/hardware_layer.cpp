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
#include "dls2/core_framework/hardware_layer.hpp"
#include "dls2/core_framework/options.hpp"
#include <sys/types.h>
#include <sys/wait.h>

using namespace dls;

// =============================================================================
// Constructors
// =============================================================================
HardwareLayer::HardwareLayer(std::string ID_) 
	: AppLayer(ID_)
{
	std::cout << "#### HARDWARE INTERFACE ####" << std::endl;

    command_manager.addCommand<std::string>
	(
		"loadHardware",
		"Load thel HAL of a robot",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			if(!this->loadComponentFromDisk(type))
                return false;

            return true;
        }),
		{{0,1}},
		true
	);

    command_manager.addCommand<>
	(
		"unloadHardware",
		"Terminates the HAL of the robot",
		std::function<bool()>([&]()->bool
        {
            for (auto &elem : this->components)
            {
                elem.second->stop();
                this->removeComponent(elem.second->getID());
            }
            return true;
		}),
		{{1,0}},
		true
	);
}

HardwareLayer::~HardwareLayer()
{
	std::cout << "#### HARDWARE INTERFACE OFF ####" << std::endl;
}


// =============================================================================
// Member Functions
// =============================================================================
HardwareLayer::Status HardwareLayer::run()
{
	setStatus(Status::RUNNING);

	while(!this->shouldQuit())
	{
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
	}
	return getStatus();
}

HardwareLayer::Status HardwareLayer::shutdown()
{
	this->should_quit = true;

	setStatus(Status::STOP);

	return getStatus();
}