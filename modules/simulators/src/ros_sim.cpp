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
#include "ros/ros.h"

#include <signal.h>


using namespace dls;

ROSSim::ROSSim(std::string ID) 
	: SimulatorBase(ID)
{

    int argc;
    ros::init(argc, {}, "master_test");

	command_manager.addCommand<void>
	(
		"launchCore",
		"Launches roscore",
		std::function<void()>([&](){
			this->launchCore();
		}),
		{{1,2}},
		true
	);

	command_manager.addCommand<void>
	(
		"exitCore",
		"Terminates roscore",
		std::function<void()>([&](){
			this->exitCore();
		}),
		{{2,1}},
		true
	);

    command_manager.addCommand<void>
	(
		"launchSim",
		"Launches the simulation backend",
		std::function<void()>([&](){
			this->launchSim();
		}),
		{{2,3}},
		true
	);

	command_manager.addCommand<void>
	(
		"exitSim",
		"Terminates simulation",
		std::function<void()>([&](){
			this->exitSim();
		}),
		{{3,2}},
		true
	);

	command_manager.addCommand<int>
	(
		"freezeBase",
		"Toggle the RUN/FREEZE state of the simulation",
		std::function<int()>{[&](){
			return system("rosservice call /hyq/freeze_base");
		}},
		{{3,3}},
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

bool ROSSim::launchCore()
{
    if(!ros::master::check()){
	    this->processes.insert({"roscore", new boost::process::child("/opt/ros/noetic/bin/roscore")});
	    this->processes.at("roscore")->detach();
    }

    int time = 0;
    while(!ros::master::check() && time < 10){
        sleep(1);
        time++;
    }

    if(time > 10){
        std::cout << "roscore took too much time to start" << std::endl;
        return false;
    }
    return true;
}

void ROSSim::exitCore()
{
	auto proc = this->processes.find("roscore");

	if ((proc == this->processes.end()) || (proc->second == nullptr))
		return;
		
	kill(proc->second->id(), SIGINT);
	this->processes.erase("roscore");
}

AppLayerComponent::Status ROSSim::run()
{
    command_manager.changeLevel(1);
    return this->getStatus();
}

AppLayerComponent::Status ROSSim::stop()
{
    exitSim();
    exitCore();
    command_manager.changeLevel(0);

    return this->getStatus();
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