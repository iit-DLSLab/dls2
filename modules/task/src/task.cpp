#include "dls2/task/task.hpp"
#include "dls2/util/utils.hpp"
namespace tasks{
    Task::Task(const std::string &ID) 
    : PeriodicAppPlugin(ID)
    , sm_watcher(ID+"_task_sm_watcher")
    {}

    Task::~Task()
    { }

    bool Task::checkActivation(){
        if(basicActivationChecks() && checkWorkConditions() && initTask())
            return true;
        return false;
    }

    bool Task::checkWorkConditions(){
        // Working condition: for each writer, there is at list one app in run or activation state listening to it
        bool can_work = true;
        // get names of applications among the data readers names and check their states
        std::map<std::string, std::vector<std::string>> apps_not_ready;
        std::map<std::string, std::vector<std::string>> apps_ready;

        {
            std::lock_guard<std::mutex> lock(output_info_mutex_);
            for(const auto& info : this->output_info_){
                const auto& writer = info.writer;
                // writer topic name
                auto topic_name = writer->getTopic().first;
                apps_ready[topic_name]={};
                apps_not_ready[topic_name]={};
                // get matched data readers names
                auto data_readers = writer->getMatchedReaders();
                if(!data_readers.empty()){
                    for (const auto &name : data_readers){
                        size_t idx = name.find("::");
                        const std::string updated_name = name.substr(0, idx);
                        // if app is found in data readers name 
                        // and its state is "activation" or "run"
                        // the writer is ready
                        if(updated_name!=this->getID() && sm_watcher.app_states.find(updated_name) != sm_watcher.app_states.end()){
                            if(sm_watcher.app_states[updated_name].first == "activation" || sm_watcher.app_states[updated_name].first == "run"){//if apps among matching in run or activation state --> app ready
                                apps_ready[topic_name].push_back(updated_name);
                            }
                            else{// if apps among matching but not in run or activation state --> app listening but not ready
                                apps_not_ready[topic_name].push_back(updated_name);
                            }
                        }
                    }
                }
            }
        }
        // ** NOTIFY USER **
        // if for each writer there is at list one app in run or activation state, the task can run properly
        for(auto &[name, apps]: apps_ready){
            if(apps.size()==0){
                can_work = false;
            }
        }
        if(can_work){
            std::stringstream ss;
            ss << "RUNNING TASK: " << getID() << " using the following applications: ";
            for(auto &[name, apps]: apps_ready){
                ss << apps[0] << " ";
            }
            ss << "\n";
            std::cout << ss.str();
        }
        else{
            activation_message << "RUNNING TASK: " << getID() << ", but: \n";
            for(auto &[name, apps]: apps_ready){
                // if the apps vectors are empty, no one is listening on the writer topic
                if(apps_not_ready[name].size()==0 && apps_ready[name].size()==0)
                    activation_message << "\""<<name << "\" topic: " <<" no application is listening"<< ".\n";
                // else if the apps_ready is empty and apps_not_ready is not, the apps are not in run or activation state
                else if(apps_ready[name].size()==0 && apps_not_ready[name].size()!=0){
                    activation_message << "\""<<name << "\" topic: " << " no application is in run or activation state listening to it"<< ". ";
                    activation_message << "Available applications are: ";
                    for(auto &app: apps_not_ready[name]){
                        activation_message << app << " ";
                    }
                    activation_message << "\n";
                }
            }
        }

        return true;
    }

    bool Task::deactivation(const std::chrono::system_clock::time_point&){
        // move command state machine to level 0 because the commands are not yet integrated inside the application state machine
        command_manager.changeLevel(0);
        return true;
    }

    void Task::stopTask(){
        sm.raiseEvent(sm.deactivation_request);
        this->app_logger.info("TASK IS COMPLETED.");
    }
}