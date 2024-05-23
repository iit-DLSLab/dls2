#ifndef TASK_HPP
#define TASK_HPP

#include "dls2/plugin/periodic_app_plugin.hpp"
#include "dls2/state_machine/state_machine_watcher.hpp"

namespace tasks{
    class Task : public PeriodicAppPlugin
    {
    public:
        Task(const std::string &ID);
        ~Task();

        bool checkActivation() override;

        virtual bool initTask()=0;
        
        virtual bool deactivation(const std::chrono::system_clock::time_point&);

        void stopTask();   

        bool checkWorkConditions();     
        
    protected:
        state_machine::StateMachineWatcher sm_watcher;
    };
}
#endif