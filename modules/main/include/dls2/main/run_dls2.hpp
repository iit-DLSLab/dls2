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
        void change_process_name(char **argv, const std::string &name);
        void launchSupervisor();
        void launchLayers();
        void runStartup(const std::string &);
        bool runLayer(const std::string &, const std::string &);
        void launchServers();
        void launchParamServer();
        void launchActionServers();
        void launchProcedures();
        void launchGenerics();
        void launchTasks();
        static void shutdown_all(int);

        static CommandManager command_manager;
        static bool should_quit;
        std::map<std::string, std::shared_ptr<AppData>> layers;
        //! List of discovery servers
        std::vector<std::shared_ptr<DDSParticipant>> servers;
        // add state machine watcher
        state_machine::StateMachineWatcher sm_watcher;
        DDSParticipant ddspart;
    };
}
#endif
