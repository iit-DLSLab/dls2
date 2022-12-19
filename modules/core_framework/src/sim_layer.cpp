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
#ifndef SIM_LAYER_CPP
#define SIM_LAYER_CPP

#include "dls2/core_framework/sim_layer.hpp"
#include <signal.h>

using namespace dls;

SimLayer::SimLayer(std::string ID) 
	: AppLayer(ID)
{
	command_manager.addCommand<std::string>
	(
		"loadSimulator",
		"Launches a simulator",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			if(!this->loadComponentFromDisk(type))
                return false;

            return true;
        }),
		{{0,1},{1,1}},
		true
	);

	command_manager.addCommand<>
	(
		"removeSimulator",
		"Terminates a instance of a simulator",
		std::function<bool(std::string)>([&](std::string name_)->bool
        {
            return removeSimulator(name_);
		}),
		{{1,0}},
		true
	);

    command_manager.addCommand<>
	(
		"list",
		"Lists all simulators running",
		std::function<bool()>([&]()->bool
        {
            std::cout << std::endl;
            for(auto elem : this->components){
                std::cout << elem.first << std::endl;
            }
            
            return true;
		}),
		{{1,1}},
		true
	);
}

SimLayer::~SimLayer()
{ }

// =============================================================================
// Interface Override
// =============================================================================

AppLayer::Status SimLayer::run()
{
	while(!this->should_quit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return getStatus();
}


AppLayer::Status SimLayer::shutdown()
{
	for(auto pair : this->components)
		pair.second->stop();

	this->should_quit = true;

	setStatus(Status::STOP);
	return getStatus();
}

bool SimLayer::removeSimulator(std::string name_)
{
	if(this->components.count(name_)){
        this->components[name_]->stop();
        this->removeComponent(name_);
        if(!this->components.size())
            return true;
    }
    else
    {
        std::cout << "There is no instance of the simulator with " << name_ << " name running" << std::endl;
    }
    return false;
}

#endif // SIM_LAYER_CPP