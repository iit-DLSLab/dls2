#include "dls2/state_machine/state_machine_watcher.hpp"
#include <iostream>
#include <thread>

namespace state_machine
{
    StateMachineWatcher::StateMachineWatcher(const std::string &name)
        : dds_sm_watcher(name, dls::domains::layers, eprosima::fastrtps::rtps::DiscoveryProtocol_t::SUPER_CLIENT)
    {
        eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
        qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
        qos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;

        dds_sm_watcher.addReader("state_machine_watcher",
                                 dls::topics::state_machine,
                                 std::function<void(void *)>{[&](void *msg)
                                                             {
                                                                 auto component = static_cast<StateMachineMsg *>(msg);
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
        while (!stop_wait && app_states.find(app_name) == app_states.end())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        // wait state
        while (!stop_wait && app_states.at(app_name).first != state)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        return true;
    }
}