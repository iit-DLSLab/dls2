
#ifndef SUPERVISOR_CPP
#define SUPERVISOR_CPP

#include "dls2/supervisor/supervisor.hpp"
#include "dls2/domains/domains.hpp"

namespace dls
{

	Supervisor::Supervisor(std::string ID)
    : ID_(ID)
    , scout_info(ID)
    , layersLink(ID, dls::domains::layers, eprosima::fastrtps::rtps::DiscoveryProtocol_t::SUPER_CLIENT)
    , command_manager(ID)
	{
        eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
        qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
        qos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;

        layersLink.addReader(  "watcher", 
                                dls::topics::state_machine, 
                                std::function<void(void*)>{[&](void* msg){
                                    auto component = static_cast<StateMachineMsg*>(msg);
                                    std::string name = component->app_name();
                                    std::string state = component->state();
                                    bool realtime = component->realtime();

                                    if(app_states.find(name) == app_states.end()){
                                        app_states[name] = std::make_pair(state, realtime);
                                    }
                                    else{
                                        app_states[name].first = state;
                                        app_states[name].second = realtime;
                                    }
                                }},
                                qos);
        
       	command_manager.addCommand<>
        (
            "info",
            "Info",
            std::function<bool()>([&]()->bool
            {
                std::string info = "\n";
                for (const auto & [key, value] : app_states){
                    info += key + " " + value.first + " ";
                    if(value.second){
                        info += "RT";
                    }
                    else{
                        info += "NRT";
                    }
                    info+="\n";
                }
                scout_info << info << std::endl;
                return true;
            }),
            {},
            true
        );
    }

    Supervisor::~Supervisor()
	{ }

    std::string Supervisor::getID()
    {
        return this->ID_;
    }

    int Supervisor::getNumLayers()
    {
        // return layersLink.getParticipants().size()-1;
        auto layers = layersLink.getParticipants();

        return std::count_if(layers.begin(), layers.end(), [](std::string s) { return s.find("Layer") != std::string::npos; });
    }

    std::vector<std::string> Supervisor::getLayersNames()
    {
        return layersLink.getParticipants();
    }

    bool Supervisor::containsLayer(std::string name)
    {
        auto layers = layersLink.getParticipants();

        return (std::find(layers.begin(), layers.end(), name) != layers.end()); 
    }


}

#endif /* end of include guard: SUPERVISOR_CPP */