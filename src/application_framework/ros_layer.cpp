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
#include "dls2/application_framework/ros_layer.phpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <ros/ros.h>
#include <ros/master.h>

using namespace dls;

// =============================================================================
// Constructors
// =============================================================================
RosLayer::RosLayer() :
    AppLayer("ros_layer"),
    roscore_pid(0),
    scout("ros_layer"),
    command_manager()
{
    forkRosCore();
    command_manager.addCommand<void, ARGVOID>
    (
        "ros_layer",
        "launchRosCore",
        "Launches the ros core",
        std::function<void(ARGVOID)>
        (
            [&](ARGVOID)
            {
                forkRosCore();
            }
        )
    );
    ros::init(Options::argc,Options::argv,"DLS2_ROS_Layer",ros::init_options::NoSigintHandler);
    ros::NodeHandle node;
    ros::spin();
}

RosLayer::~RosLayer()
{

}


// =============================================================================
// Member Functions
// =============================================================================
RosLayer::Status RosLayer::run()
{
    return getStatus();
}

RosLayer::Status RosLayer::shutdown()
{
    ros::shutdown();
    return getStatus();
}

std::string RosLayer::where()
{
    std::string ret = std::string("pid=")+std::to_string(this->roscore_pid)+'\n';
    ret += "ros master check: " + std::to_string(ros::master::check());
    return ret;
}

//TODO this is copy and pasted remove from here
void change_process_name(char **argv, const std::string &name)
{
    // change info in /proc/$pid/cmdline
    int alloted_space = std::strlen(argv[0]);
    std::strncpy(argv[0], name.c_str(), alloted_space);

    // change info in /proc/$pid/status
    prctl(PR_SET_NAME, name.c_str());
}

void RosLayer::forkRosCore()
{
    pid_t pid = fork(); // TODO error handling on fork
    if(pid == 0)
    {
        change_process_name(Options::argv, "roscore");
        // auto ret = system("roscore > /dev/null");
        auto ret = system("roscore");
        _exit(ret);
    } else {
        this->roscore_pid=pid;
    }
}


