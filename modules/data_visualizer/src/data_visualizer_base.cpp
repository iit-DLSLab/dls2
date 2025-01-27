#include "dls2/data_visualizer/data_visualizer_base.hpp"

namespace dls
{
    DataVisualizerBase::DataVisualizerBase(const std::string& ID,
                                           const std::shared_ptr<robotlib::RobotBase> robot)
        : PeriodicApp(ID)
        , dds_participant_(std::make_shared<dls::DDSParticipant>("DataVisualizerBase::" + ID, dls::domains::signals, eprosima::fastdds::rtps::DiscoveryProtocol::SUPER_CLIENT))
        , robot_(robot)
    {
        std::ifstream scene_schema("/usr/lib/dls2/data_visualizer/json/SceneUpdate.json");
        scene_schema_parsed_ = nlohmann::json::parse(scene_schema);

        scene_channel_parameters_ = {.topic = "",
                                     .encoding = "json",
                                     .schemaName = "foxglove.SceneUpdate",
                                     .schema = scene_schema_parsed_.dump(),
                                     .schemaEncoding = "jsonschema"};
    }

    DataVisualizerBase::~DataVisualizerBase(){}

    std::shared_ptr<dls::DDSParticipant> DataVisualizerBase::getParticipant()
    {
    	return dds_participant_;
    }

    std::shared_ptr<robotlib::RobotBase> DataVisualizerBase::getRobot()
    {
    	return robot_;
    }
} //namespace dls