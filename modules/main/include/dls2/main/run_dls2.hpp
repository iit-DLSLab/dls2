#ifndef RUN_DLS2_HPP
#define RUN_DLS2_HPP

#include "fastdds/dds/domain/DomainParticipant.hpp"
#include "dls2/state_machine/state_machine_watcher.hpp"
#include "dls2/command/command_manager.hpp"
#include "dls2/application/app_data.hpp"

namespace dls
{
    class RunDLS2
    {
    public:
        RunDLS2(int argc, char **argv);
        ~RunDLS2();

        void run();

    private:
        enum class INSTRUCTION_OPTIONS
        {
            LAYER=0,
            COMMAND
        };

        class StartupConfigOptions
        {
            public:
                const std::string LAYERS = "layers";
                const std::string HARDWARES = "hardwares";
                const std::string XENO_HARDWARES = "xeno_hardwares";
                const std::string CONTROLLERS = "controllers";
                const std::string PYTHON_CONTROLLERS = "python_controllers";
                const std::string MOTION_GENERATORS = "motion_generators";
                const std::string ESTIMATORS = "estimators";
                const std::string GENERIC_PERIODIC_APP_PLUGINS = "generic_periodic_app_plugins";
                const std::string PYTHON_PERIODIC_APPS = "python_periodic_apps";
                const std::string GENERIC_APP_PLUGINS = "generic_app_plugins";
                const std::string ROBOT_NAME = "robot_name";
                const std::string ROBOT_SPAWNING_HEIGHT = "robot_spawning_height";
                const std::string LOAD_MODEL = "load_model";
                const std::string ACTIVATE = "activate";
                const std::string RUN_SERVERS = "run_servers";
                const std::string RUN_SUPERVISOR = "run_supervisor";
                
                const std::vector<std::string> ALL_POSSIBLE_OPTIONS = {LAYERS, HARDWARES, XENO_HARDWARES, CONTROLLERS, PYTHON_CONTROLLERS, MOTION_GENERATORS, ESTIMATORS, GENERIC_PERIODIC_APP_PLUGINS, PYTHON_PERIODIC_APPS, GENERIC_APP_PLUGINS, ROBOT_NAME, ROBOT_SPAWNING_HEIGHT, LOAD_MODEL, ACTIVATE, RUN_SERVERS, RUN_SUPERVISOR};
        } startup_config_entries;

        static CommandManager command_manager;
        static bool should_quit;
        std::map<std::string, std::shared_ptr<AppData>> layers;
        //! List of discovery servers
        std::vector<std::shared_ptr<DDSParticipant>> servers;
        // add state machine watcher
        state_machine::StateMachineWatcher sm_watcher;
        DDSParticipant ddspart;
        
        std::vector<std::string> layer_names;
        std::map<std::string, std::string> allowed_app_to_layer_mapping;
        std::map<std::string, std::string> allowed_app_to_loading_command;
        // define a map to store: application_type: {layer/node responsible for launching the app, command_to_load} 
        std::map<std::string, std::vector<std::string>> instructions_set;


        std::string print_header;

        void change_process_name(char **argv, const std::string &name);
        void launchSupervisor();
        void launchLayers();
        void runStartup(const std::string &);
        bool runLayer(const std::string &, const std::string &);
        void launchServers();
        void launchSingleServer(const std::string& ip, int port);
        std::vector<std::string> collectLayersToRun(const YAML::Node &config);
        std::map<std::string, std::vector<std::string>> collectApplicationsToRun(const YAML::Node &config);
        void validateStartupConfig(const YAML::Node &config);

        static void shutdown_all(int);
    };
}
#endif