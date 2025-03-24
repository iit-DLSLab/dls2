#include "dls2/state_machine/state_machine_watcher.hpp"
#include "dls2/util/utils.hpp"

#include <iostream>
#include <thread>

namespace state_machine
{
    StateMachineWatcher::StateMachineWatcher(const std::string &name)
        : dds_sm_watcher(name, dls::domains::layers, eprosima::fastdds::rtps::DiscoveryProtocol::SUPER_CLIENT)
    {
        eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
        qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
        qos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;

        dds_sm_watcher.addReader("state_machine_watcher",
                                 dls::topics::state_machine,
                                 std::function<void(void *)>{[&](void *msg)
                                                             {
                                                                 auto component = static_cast<StateMachineMonitor *>(msg);
                                                                 std::string name = component->app_name();
                                                                 std::string state = component->state();
                                                                 bool realtime = component->realtime();

                                                                 if (app_states.find(name) == app_states.end())
                                                                 {
                                                                     app_states[name] = std::make_pair(state, realtime);
                                                                 }
                                                                 else
                                                                 {
                                                                     app_states[name].first = state;
                                                                     app_states[name].second = realtime;
                                                                 }
                                                             }},
                                 qos);
    }
    StateMachineWatcher::~StateMachineWatcher() {}

    bool StateMachineWatcher::waitState(const std::string &app_name, const std::string &state, bool& stop_wait) const
    {       
        // wait app
        if(!dls::utils::wait(std::function<bool()>([&](){
                if(app_states.find(app_name) == app_states.end()){
                    return false;
                }
                return true;
            }), 5000, 2, stop_wait)){
            if(!stop_wait){
                std::cerr << app_name << " not found" << std::endl;
                return false;
            }
		}

        // wait state
        if(!dls::utils::wait(std::function<bool()>([&](){
            if(app_states.at(app_name).first != state){
                    return false;
                }
                return true;
            }), 5000, 2, stop_wait)){
            if(!stop_wait){
                std::cerr << app_name << " not found in state " << state << std::endl;
                return false;}
            }

        return true;
    }


    bool StateMachineWatcher::waitState(const std::string &app_name, const std::string &state, std::atomic_bool& stop_wait) const
    {       
        // wait app
        if(!dls::utils::wait(std::function<bool()>([&](){
                if(app_states.find(app_name) == app_states.end()){
                    return false;
                }
                return true;
            }), 5000, 2, stop_wait)){
            if(!stop_wait.load()){
                std::cerr << app_name << " not found" << std::endl;
                return false;}
            }

        // wait state
        if(!dls::utils::wait(std::function<bool()>([&](){
            if(app_states.at(app_name).first != state){
                    return false;
                }
                return true;
            }), 5000, 2, stop_wait)){
            if(!stop_wait.load()){
                std::cerr << app_name << " not found in state " << state << std::endl;
                return false;}
            }

        return true;
    }

    bool StateMachineWatcher::waitApp(const std::string &app_name, bool& stop_wait) const
    {       
        // wait app
        if(!dls::utils::wait(std::function<bool()>([&](){
                if(app_states.find(app_name) == app_states.end()){
                    return false;
                }
                return true;
            }), 5000, 2, stop_wait)){
            if(!stop_wait){
                std::cerr << app_name << " not found" << std::endl;
                return false;
            }
		}
        return true;
    }


    bool StateMachineWatcher::waitApp(const std::string &app_name, std::atomic_bool& stop_wait) const
    {       
        // wait app
        if(!dls::utils::wait(std::function<bool()>([&](){
                if(app_states.find(app_name) == app_states.end()){
                    return false;
                }
                return true;
            }), 5000, 2, stop_wait)){
            if(!stop_wait.load()){
                std::cerr << app_name << " not found" << std::endl;
                return false;}
            }
        std::cout << dds_sm_watcher.getName() << std::endl;
        return true;
    }

    bool StateMachineWatcher::findApp(const std::string &app_name) const
    {
        if(app_states.find(app_name) == app_states.end()){
            return false;
        }
        return true;
    }

    bool StateMachineWatcher::findState(const std::string &app_name, const std::string &state) const
    {
       if(!findApp(app_name) || app_states.at(app_name).first != state)
           return false;
        return true;
    }
}