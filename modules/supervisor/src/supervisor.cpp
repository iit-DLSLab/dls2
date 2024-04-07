
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
	{
        eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
        qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
        qos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;

        layersLink.addReader(  "watcher", 
                                dls::topics::state_machine, 
                                std::function<void(void*)>{[&](void* msg){
                                    auto component = static_cast<StateMachineMsg*>(msg);
                                    scout_info <<component->app_name() << " " << component->state() << " " << component->realtime() << std::endl;
                                }},
                                qos);
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