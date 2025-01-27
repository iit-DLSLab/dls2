
#ifndef SUPERVISOR_CPP
#define SUPERVISOR_CPP

#include "dls2/supervisor/supervisor.hpp"
#include "dls2/domains/domains.hpp"

namespace dls
{
	Supervisor::Supervisor(std::string ID)
    : Layer(ID, 500)
    , ddspart_layers(ID, dls::domains::layers, eprosima::fastdds::rtps::DiscoveryProtocol::SUPER_CLIENT)
    , state_machine_watcher(ID+"_state_machine_watcher")
	{        
       	command_manager.addCommand<>
        (
            "info",
            "Info",
            std::function<bool()>([&]()->bool
            {
                std::string info = "\n";
                for (const auto & [key, value] : state_machine_watcher.app_states){
                    info += key + " " + value.first + " ";
                    if(value.second){
                        info += "RT";
                    }
                    else{
                        info += "NRT";
                    }
                    info+="\n";
                }
                scout_sys << info << std::endl;
                return true;
            }),
            {},
            true
        );

        command_manager.addCommand<std::string>
        (
            "runProcedure",
            "Load and execute procedure",
            std::function<bool(std::string)>([&](std::string type)->bool
            {
                // if(command_manager.waitCommand("ServiceLayer","loadProcedure", sm.async_events[sm.quit_request]))
                //     command_manager.callCommand("loadProcedure", {type}, "ServiceLayer");

                if(!state_machine_watcher.waitState(type, "idle", sm.async_events[sm.quit_request])){
                    scout_warn << "Procedure " << type << " not in idle state" << std::endl;
                }

                if(command_manager.waitCommand(type, "activate", sm.async_events[sm.quit_request]))
                    command_manager.callCommand("activate", {}, type);

                return true;

            }),
            {},
            true
        );

        command_manager.addCommand<std::string>
        (
            "runTask",
            "Execute task",
            std::function<bool(std::string)>([&](std::string type)->bool
            {
                if(!state_machine_watcher.waitState(type, "idle", sm.async_events[sm.quit_request])){
                    scout_warn << "Task " << type << " not in idle state" << std::endl;
                }

                if(command_manager.waitCommand(type, "activate", sm.async_events[sm.quit_request]))
                    command_manager.callCommand("activate", {}, type);

                return true;

            }),
            {},
            true
        );

    }

    Supervisor::~Supervisor()
	{ }

    void Supervisor::monitor()
    { }

    int Supervisor::getNumLayers()
    {
        // return ddspart_layers.getParticipants().size()-1;
        auto layers = ddspart_layers.getParticipants();

        return std::count_if(layers.begin(), layers.end(), [](std::string s) { return s.find("Layer") != std::string::npos; });
    }

    std::vector<std::string> Supervisor::getLayersNames()
    {
        return ddspart_layers.getParticipants();
    }

    bool Supervisor::containsLayer(std::string name)
    {
        auto layers = ddspart_layers.getParticipants();

        return (std::find(layers.begin(), layers.end(), name) != layers.end()); 
    }
}

#endif /* end of include guard: SUPERVISOR_CPP */