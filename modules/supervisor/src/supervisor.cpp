
#ifndef SUPERVISOR_CPP
#define SUPERVISOR_CPP

#include "dls2/supervisor/supervisor.hpp"
#include "dls2/domains/domains.hpp"
#include "dls2/util/utils.hpp"


namespace dls
{
	Supervisor::Supervisor(std::string ID, const std::string& robot_name)
    : Layer(ID, 500)
    , ddspart_layers(ID, dls::domains::layers, eprosima::fastdds::rtps::DiscoveryProtocol::SUPER_CLIENT)
    , state_machine_watcher(ID+"_state_machine_watcher")
    , robot_name(robot_name)
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
                this->app_logger.info(info);
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
                    this->app_logger.warning("Procedure " + type + " not in idle state", EventID::WRONG_PROCESS_STATE);
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
            "loadPeriodicAppPlugin",
            "Load a periodic app plugin",
            std::function<bool(std::string)>([&](std::string type)->bool
            {
                return this->loadPeriodicAppPlugin(type);
            }),
            {},
            true
        );


        command_manager.addCommand<std::string>
        (
            "unloadPeriodicAppPlugin",
            "Unload a periodic app plugin",
            std::function<bool(std::string)>([&](std::string type)->bool
            {
                return this->unloadPeriodicAppPlugin(type);
            }),
            {},
            true
        );        
        
        command_manager.addCommand<std::string>
        (
            "loadPythonPeriodicApp",
            "Load a Python-backed periodic app",
            std::function<bool(std::string)>([&](std::string type)->bool
            {
                return this->loadPythonPeriodicApp(type);
            }),
            {},
            true
        );        


        sys_monitor = std::make_unique<dls::SystemResourceMonitor>(this->safety_layer_config_->resource_monitor_window_size);
    }

    Supervisor::~Supervisor()
	{ }

    void Supervisor::monitor()
    { 
        this->checkAppData(plugins);
        checkHardware();
    }

    void Supervisor::checkHardware()
    {
        sys_monitor->monitor();

        auto mem_usage = sys_monitor->getMemUsage();
        auto temperature = sys_monitor->getTemperature();
        auto cpus_usage = sys_monitor->getCpusUsage();
        
        for(size_t i = 0; i < cpus_usage.size(); i++){
            if(this->safety_layer_config_->enable_cpu_usage_too_high && 
               cpus_usage.at(i) > this->safety_layer_config_->system_cpu_threshold){
                this->robust_event_notifier.notify(
                    EventID::CPU_USAGE_TOO_HIGH,
                    EventSeverity::WARNING,
                    this->getID() + ": overall cpu usage - on core n. " + std::to_string(i) + " - is " 
                        + std::to_string(cpus_usage.at(i)) + "(threshold is " + std::to_string(this->safety_layer_config_->system_cpu_threshold) + ")"
                );
            }
        }

        if(this->safety_layer_config_->enable_cpu_temp_too_high && 
            temperature.second > this->safety_layer_config_->system_cpu_temp_threshold)
        {
            this->robust_event_notifier.notify(
                EventID::CPU_TEMP_TOO_HIGH,
                EventSeverity::WARNING,
                this->getID() + ": overall cpu temperature is " 
                    + std::to_string(temperature.second) + "(threshold is " + std::to_string(this->safety_layer_config_->system_cpu_temp_threshold) + ")"
            );
        }
        
        if (this->safety_layer_config_->enable_mem_usage_too_high && 
            mem_usage > this->safety_layer_config_->mem_threshold)
        {
            this->robust_event_notifier.notify(
                EventID::MEM_USAGE_TOO_HIGH,
                EventSeverity::WARNING,
                this->getID() + ": overall memory usage is " + std::to_string(mem_usage) + 
                    " (threshold is " + std::to_string(this->safety_layer_config_->mem_threshold) + ")"
            );
        }
    }

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

    bool Supervisor::loadPeriodicAppPlugin(const std::string& name)
    {	
        if(this->plugins.find(name) != this->plugins.end())
        {
            this->app_logger.error("plugin " + name + " already loaded");
            return false;
        }

        std::shared_ptr<AppData> pData = std::make_shared<AppData>(name);
        
        // launch the process
        char *child_process_launcher = std::getenv("DLS_CHILD_PROCESS_LAUNCHER");
        if(!child_process_launcher)
        {
            this->app_logger.error(
                "env variable DLS_CHILD_PROCESS_LAUNCHER not "
                "defined.  This is probably an error with the launch script"
        );
            return false;
        }

        pData->proc = std::make_shared<boost::process::child>(std::vector<std::string>({
            child_process_launcher,
            pData->getID(),
            name,
            "periodic_app_plugin",
            robot_name
        }));

        if (pData->proc == nullptr){
            std::cout << "Task " << name <<" failed to launch: nullptr" << std::endl;
            return false;
        }
        
        pData->proc->detach();
        
        this->plugins.emplace(pData->getID(), pData);

        return true;
    }

    bool Supervisor::loadPythonPeriodicApp(const std::string& name)
    {
        if(this->plugins.find(name) != this->plugins.end())
        {
            this->app_logger.error("plugin " + name + " already loaded");
            return false;
        }

        std::shared_ptr<AppData> pData = std::make_shared<AppData>(name);

        char *child_process_launcher = std::getenv("DLS_CHILD_PROCESS_LAUNCHER");
        if(!child_process_launcher)
        {
            this->app_logger.error(
                "env variable DLS_CHILD_PROCESS_LAUNCHER not "
                "defined.  This is probably an error with the launch script"
            );
            return false;
        }

        pData->proc = std::make_shared<boost::process::child>(std::vector<std::string>({
            child_process_launcher,
            pData->getID(),
            name,
            "python_periodic_app",
            robot_name
        }));
        
        if (pData->proc == nullptr){
            std::cout << "Task " << name <<" failed to launch: nullptr" << std::endl;
            return false;
        }
        
        pData->proc->detach();
        
        this->plugins.emplace(pData->getID(), pData);

        return true;
    }

    bool Supervisor::loadAppPlugin(const std::string& name)
    {	
        if(this->plugins.find(name) != this->plugins.end())
        {
            this->app_logger.error("plugin " + name + " already loaded");
            return false;
        }

        std::shared_ptr<AppData> pData = std::make_shared<AppData>(name);
        
        // launch the process
        char *child_process_launcher = std::getenv("DLS_CHILD_PROCESS_LAUNCHER");
        if(!child_process_launcher)
        {
            this->app_logger.error(
                "env variable DLS_CHILD_PROCESS_LAUNCHER not "
                "defined.  This is probably an error with the launch script"
            );
            return false;
        }

        pData->proc = std::make_shared<boost::process::child>(std::vector<std::string>({
            child_process_launcher,
            pData->getID(),
            name,
            "app_plugin",
            robot_name
        }));

        if (pData->proc == nullptr){
            std::cout << "Task " << name <<" failed to launch: nullptr" << std::endl;
            return false;
        }
        
        pData->proc->detach();
        
        this->plugins.emplace(pData->getID(), pData);

        return true;
    }

    bool Supervisor::unloadAppPlugin(const std::string& name)
    {
        auto pair_it = this->plugins.find(name);
        if(pair_it == this->plugins.end())
        {
            this->app_logger.error("plugin " + name + " not loaded");
            return false;
        }

        command_manager.callCommand("shutdown", {}, pair_it->second->getID());
        bool unloaded = false;
	    if(!utils::wait(std::function<bool()>([&](){
			if(pair_it->second->proc->running()){
				return false;
			}
			return true;
		}), 2000, 10, unloaded)){
		std::cout << "### FORCING APP PLUGIN " << pair_it->second->getID() << " EXIT ###" << std::endl;
		kill(pair_it->second->proc->id(), SIGKILL);		
	    }

	    pair_it->second->proc = nullptr;
        this->plugins.erase(pair_it->second->getID());

        return true;
    }

    bool Supervisor::unloadPeriodicAppPlugin(const std::string& name)
    {
        auto pair_it = this->plugins.find(name);
        if(pair_it == this->plugins.end())
        {
            this->app_logger.error("plugin " + name + " not loaded");
            return false;
        }

        command_manager.callCommand("shutdown", {}, pair_it->second->getID());
        bool unloaded = false;
        if(!utils::wait(std::function<bool()>([&](){
            if(pair_it->second->proc->running()){
                return false;
            }
            return true;
        }), 2000, 10, unloaded)){
            std::cout << "### FORCING PERIODIC PERIODIC APP PLUGIN " << pair_it->second->getID() << " EXIT ###" << std::endl;
            kill(pair_it->second->proc->id(), SIGKILL);		
        }
        this->plugins.erase(pair_it->second->getID());

        return true;
    }
}

#endif /* end of include guard: SUPERVISOR_CPP */