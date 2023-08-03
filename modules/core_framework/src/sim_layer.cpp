
#ifndef SIM_LAYER_CPP
#define SIM_LAYER_CPP

#include "dls2/core_framework/sim_layer.hpp"
#include <signal.h>

using namespace dls;

SimLayer::SimLayer(std::string ID) 
	: Layer(ID)
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
		"unloadSimulator",
		"Terminates a instance of a simulator",
		std::function<bool(std::string)>([&](std::string name_)->bool
        {
            return unloadSimulator(name_);
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
            scout_sys << std::endl;
            for(auto elem : this->components){
                scout_sys << elem.first << std::endl;
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

Layer::AppStatus SimLayer::run()
{
	while(!this->should_quit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return getStatus();
}


Layer::AppStatus SimLayer::shutdown()
{
	for(auto pair : this->components)
		pair.second->stop();

	this->should_quit = true;

	setStatus(AppStatus::STOP);
	return getStatus();
}

bool SimLayer::unloadSimulator(std::string name_)
{
	if(this->components.count(name_)){
        this->components[name_]->stop();
        this->removeComponent(name_);
		scout_sys << name_ << " is unloaded." << std::endl;
        if(!this->components.size())
            return true;
    }
    else
    {
        scout_err << "There is no instance of the simulator with " << name_ << " name running" << std::endl;
    }
    return false;
}

#endif // SIM_LAYER_CPP