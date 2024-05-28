#include "dls2/procedures/stand_up.hpp"
#include "dls2/util/utils.hpp"
#include "robotlib/robot_factory.hpp"

namespace procedures{
    StandUp::StandUp(const std::string &ID, const std::shared_ptr<robotlib::RobotBase> robot) 
    : AppPlugin(ID)
    , blind_state(robot)
    , sm_watcher(ID+"_sm_watcher")
    {
        // Define inputs
        // this->buildInput<dls::BlindState>(
        //     dls::topics::low_level_estimation::blind_state,
        //     &blind_state
        // );
    }

    StandUp::~StandUp()
    { }

    AppStatus StandUp::run()
    {
        // Stand up procedure
        std::cout << "!!! STAND UP PROCEDURE HAS STARTED !!!\n";

        // go in home configuration
        std::cout  << "Sending home configuration...\n";
        if(command_manager.waitCommand("actions","goHome", sm.async_events[sm.quit_request]))
            command_manager.callCommand("goHome", {}, "actions");
        if(sm_watcher.findState("pid", "idle") && command_manager.waitCommand("pid","activate", sm.async_events[sm.quit_request]))
            command_manager.callCommand("activate", {}, "pid");

        std::this_thread::sleep_for(std::chrono::milliseconds(10000));

        // activate periodic generator
        std::cout  << "Activating periodic generator...\n";
        if(sm_watcher.findState("periodic_generator", "idle") && command_manager.waitCommand("periodic_generator","activate", sm.async_events[sm.quit_request]))
            command_manager.callCommand("activate", {}, "periodic_generator");

        // freezebase
        std::cout  << "freezeBase...\n";
        if(!sm_watcher.waitState("periodic_generator", "run", sm.async_events[sm.quit_request])){
            scout_err << "periodic_generator not running\n";
            return getStatus();
        }
        if(command_manager.waitCommand("gazebo_sim","freezeBase", sm.async_events[sm.quit_request]))
            command_manager.callCommand("freezeBase", {}, "gazebo_sim");

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        // activate trunk controller
        std::cout  << "Activating trunk controller...\n";
        if(sm_watcher.findState("trunk_controller", "idle") && command_manager.waitCommand("trunk_controller","activate", sm.async_events[sm.quit_request]))
            command_manager.callCommand("activate", {}, "trunk_controller");

        sm.nextState(sm.deactivation_request);

        std::cout  << "!!! STAND UP PROCEDURE IS DONE !!!\n";
	    return getStatus();
    }

    bool StandUp::checkActivation(){
        if( basicActivationChecks()){
            // wait applications to be ready
            bool ready = true;
            if(!sm_watcher.findApp("pid") || sm_watcher.findState("pid", "quit")){
                activation_message << "pid, ";
                ready = false;
            }
            if(!sm_watcher.findApp("periodic_generator") || sm_watcher.findState("periodic_generator", "quit")){
                activation_message << "periodic_generator, ";
                ready = false;
            }
            if(!sm_watcher.findApp("trunk_controller") || sm_watcher.findState("trunk_controller", "quit")){
                activation_message << "trunk_controller";
                ready = false;
            }
            if(ready){
                return true;
            }
        }
        activation_message << " not found\n";
        return false;
    }

    bool StandUp::deactivating(){
        // move command state machine to level 0 because the commands are not yet integrated inside the application state machine
        command_manager.changeLevel(0);
        return true;
    }

    extern "C" AppPlugin *create(const std::string& ID, const std::string& robot_name)
    {
        if (robot_name == "")
        {
            std::string e = "Parameter robot_name is not defined, verify if the parameter server is running";
            throw std::runtime_error(e);
        }

        std::shared_ptr<robotlib::RobotBase> pRobot;
        try
        {
            pRobot = robotlib::RobotFactory::openRobot(robot_name);
        }
        catch (const std::exception &e)
        {
            std::cerr << "child_process: Could not open the robot " << robot_name << std::endl;
            std::cerr << e.what() << std::endl;
        }
        return new StandUp(ID, pRobot);
    }

    extern "C" void destroy(AppPlugin *p)
    {
            delete p;
    }
}