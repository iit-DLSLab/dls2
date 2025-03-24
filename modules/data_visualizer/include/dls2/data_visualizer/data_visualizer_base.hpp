#ifndef DATA_VISUALIZER_BASE_HPP
#define DATA_VISUALIZER_BASE_HPP

#include "dls2/application/periodic_app.hpp"
#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/signal/signal_reader.hpp"

#include "robotlib/robot_factory.hpp"

#include <foxglove/websocket/base64.hpp>
#include <foxglove/websocket/server_factory.hpp>
#include <foxglove/websocket/websocket_notls.hpp>
#include <foxglove/websocket/websocket_server.hpp>

#include <nlohmann/json.hpp>
#include <fstream>

#include <atomic>
#include <chrono>
#include <cmath>
#include <csignal>
#include <iostream>
#include <memory>
#include <queue>
#include <thread>
#include <unordered_set>

namespace dls
{
    class DataVisualizerBase : public PeriodicApp
    {
    public:
		typedef DataVisualizerBase *create_t(std::string);
		typedef void destroy_t(DataVisualizerBase*);

        DataVisualizerBase(const std::string& ID,
						   const std::shared_ptr<robotlib::RobotBase> robot);
        virtual ~DataVisualizerBase();

		std::shared_ptr<dls::DDSParticipant> getParticipant();
		std::shared_ptr<robotlib::RobotBase> getRobot();

	protected:
		virtual void run(const std::chrono::system_clock::time_point& time) = 0;

        // DDS Participant used to discover dynamic topics
		std::shared_ptr<dls::DDSParticipant> dds_participant_;
		const std::shared_ptr<robotlib::RobotBase> robot_;

        // SceneUpdate JSON schema used for visualizing entities
  		nlohmann::json scene_schema_parsed_{};

        // Common channel parameters for the entities that use the SceneUpdate JSON schema
        foxglove::ChannelWithoutId scene_channel_parameters_{};

        // Vector of ChannelId (these need to be univoque) that are published on Foxglove
        std::vector<foxglove::ChannelId> channels_id_{};

        // Structure topics_data_ = {topic_name, channel_id}
        std::map<std::string, foxglove::ChannelId> topics_data_{};
    };
} //namespace dls

#endif