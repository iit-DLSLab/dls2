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
#ifndef ROS_SIM_CPP
#define ROS_SIM_CPP

#include "dls2/simulators/ros_sim.hpp"

#include <signal.h>


using namespace dls;

ROSSim::ROSSim(std::string ID) 
	: SimulatorBase(ID)
{
	pComManager->addCommand<void>
	(
		"launchCore",
		"Launches roscore",
		std::function<void()>([&](){
			this->launchCore();
		}),
		{{0,1}},
		true
	);

	pComManager->addCommand<void>
	(
		"exitCore",
		"Terminates roscore",
		std::function<void()>([&](){
			this->exitCore();
		}),
		{{1,0}},
		true
	);

    pComManager->addCommand<void>
	(
		"launchSim",
		"Launches the simulation backend",
		std::function<void()>([&](){
			this->launchSim();
		}),
		{{1,2}},
		true
	);

	pComManager->addCommand<void>
	(
		"exitSim",
		"Terminates simulation",
		std::function<void()>([&](){
			this->exitSim();
		}),
		{{2,1}},
		true
	);

	pComManager->addCommand<int>
	(
		"freezeBase",
		"Toggle the RUN/FREEZE state of the simulation",
		std::function<int()>{[&](){
			return system("rosservice call /hyq/freeze_base");
		}},
		{{2,2}},
		true
	);
}

void ROSSim::launchSim()
{
	this->processes.insert({"simulation", new boost::process::child("roslaunch robot_bringup hyq.launch")});	
	this->processes.at("simulation")->detach();
}

void ROSSim::exitSim()
{
	auto proc = this->processes.find("simulation");

	if ((proc == this->processes.end()) || (proc->second == nullptr))
		return;
		
	kill(proc->second->id(), SIGINT);
	this->processes.erase("simulation");
}

void ROSSim::launchCore()
{
	this->processes.insert({"roscore", new boost::process::child("/opt/ros/noetic/bin/roscore")});
	this->processes.at("roscore")->detach();
}

void ROSSim::exitCore()
{
	auto proc = this->processes.find("roscore");

	if ((proc == this->processes.end()) || (proc->second == nullptr))
		return;
		
	kill(proc->second->id(), SIGINT);
	this->processes.erase("roscore");
}



// the class factories
extern "C" ROSSim* create(std::string name) 
{
    return new ROSSim(name);
}

extern "C" void destroy(ROSSim* p) 
{
    delete p;
}
#endif /* end of include guard: ROS_SIM_CPP */