#define MCAP_IMPLEMENTATION
#include <foxglove/foxserver.hpp>

#include <fstream>
#include <Eigen/Dense>

using json = nlohmann::json;
using namespace boost;
using namespace dls;

static uint64_t nanosecondsSinceEpoch() {
    return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count());
}

FoxServer::FoxServer()
    : server_thread_(nullptr)
    , webserver_(8765, "example server")
    , dds_link_("FoxServer::monitor", dls::domains::signals, false)
{
    this->server_thread_ = std::make_shared<std::thread>(&FoxServer::serverFunc, this);
    dds_link_.setTopicListener(this);

    this->set_timer_ = [&] {
        this->timer_ = this->webserver_.getEndpoint().set_timer(30, [&](std::error_code const& ec) {
            if (ec) 
            {
                std::cerr << "timer error: " << ec.message() << std::endl;
                return;
            }
            for (auto const &chan: this->timer_flags_)
            {
                std::unique_lock<std::mutex> lock(this->send_flags_mutex_);
                this->send_flags_.erase(chan);
            }

            this->set_timer_();
        });
    };

    this->set_timer_();
}

FoxServer::~FoxServer()
{
    // Finish writing the MCAP file
    if(mcap_ongoing_recording_)
    {
        mcap_writer_.close();
        mcap_msg_ = {};
        mcap_topics_channels_.clear();
    }

    server_thread_->join();

    webserver_.stop();
}

void FoxServer::record_mcap_log(bool record_mcap, const std::string& timestamp)
{
    // Start recording a new MCAP log file
    if(record_mcap)
    {
        // Close the current MCAP log file before recording, in case you did not stop the previous one
        if(mcap_ongoing_recording_)
        {
            mcap_writer_.close();
            mcap_msg_ = {};
            mcap_topics_channels_.clear();
        }

        std::cout << "Start recording an MCAP log" << std::endl;

        // Initialize an MCAP writer with the "json" profile and write the MCAP file
        auto mcap_writer_status = mcap_writer_.open("mcap_log_" + timestamp + ".mcap", mcap::McapWriterOptions("json"));

        if (!mcap_writer_status.ok())
        {
          std::cerr << "Failed to open the MCAP file for writing: " << mcap_writer_status.message << std::endl;
        }
    }
    // Stop recording the current MCAP log file
    else
    {
        if(mcap_ongoing_recording_)
        {
            std::cout << "Stop recording the MCAP log" << std::endl;

            mcap_writer_.close();
            mcap_msg_ = {};
            mcap_topics_channels_.clear();
        }
    }

    mcap_ongoing_recording_ = record_mcap;
}

void FoxServer::serverFunc()
{
    webserver_.run();

    std::cout << "#### Foxglove Server Stopped #####" << std::endl;
} 

void FoxServer::on_topic_discovery(const std::string& topic_name, const std::string& type_name)
{
    if (type_name.find("eprosima::fastdds::statistics::") != std::string::npos)
        return;

    std::cout << "Topic discovered: " << topic_name << " [ " << type_name << " ]" << std::endl;

    auto type_ = dds::get_type_registered_(type_name);

    auto jsonPair = dds::createJsonSchema(topic_name, type_);

    auto channel = this->webserver_.addChannel({
        topic_name,
        "json",
        jsonPair.first["title"],
        jsonPair.first.dump()
    });

    this->timer_flags_.insert(channel);

    this->dds_link_.addReader(
            topic_name,
            dls::topicType({topic_name, eprosima::fastdds::dds::TypeSupport(new eprosima::fastrtps::types::DynamicPubSubType(type_))}),
            std::function<void(void *)>{[&, topic_name, type_name, jsonPair, channel](void *tuple)
            {
                std::unique_lock<std::mutex> lock(this->send_flags_mutex_);
                auto type_ = dds::get_type_registered_(type_name);

                if(this->send_flags_.find(channel) != this->send_flags_.end())
                    return;

                nlohmann::json jsonVar = jsonPair.second;

                dds::getDataToJson(topic_name, type_, (eprosima::fastrtps::types::DynamicData*) tuple, jsonVar);

                webserver_.sendMessage(channel, nanosecondsSinceEpoch(), jsonVar.dump());
                this->send_flags_.insert(channel);

                // Fill the message to be sent on the MCAP channel
                if(mcap_ongoing_recording_)
                {
                    // Apparently, it needs to store the value in a separate variable before filling the mcap_msg_.data field
                    std::string serialized_json = jsonVar.dump();

                    // Update the channel ID, add schema and channel to the writer, only if:
                    // - new topics are discovered
                    // - a new MCAP log is started after a previous one has been stopped
                    if (mcap_topics_channels_.find(topic_name) == mcap_topics_channels_.end())
                    {
                        mcap::Schema mcap_schema(type_name, "jsonschema", jsonPair.first.dump());
                        mcap_writer_.addSchema(mcap_schema);

                        mcap::Channel mcap_channel(topic_name, "json", mcap_schema.id);
                        mcap_writer_.addChannel(mcap_channel);

                        mcap_topics_channels_.insert(std::pair<std::string, mcap::ChannelId>(topic_name, mcap_channel.id));
                    }

                    mcap_msg_.channelId = mcap_topics_channels_.find(topic_name)->second;
                    mcap_msg_.logTime = nanosecondsSinceEpoch(); // Required nanosecond timestamp
                    mcap_msg_.publishTime = mcap_msg_.logTime; // Set to logTime if not available
                    mcap_msg_.data = reinterpret_cast<const std::byte*>(serialized_json.data());
                    mcap_msg_.dataSize = serialized_json.size();

                    auto mcap_writer_output = mcap_writer_.write(mcap_msg_);

                    if (!mcap_writer_output.ok())
                    {
                        std::cerr << "Failed to write the MCAP message: " << mcap_writer_output.message << std::endl;
                    }
                }
            }}
    );

    if(topic_name == "blind_state")
    {
        // Create frames
        std::ifstream jsonFrameSchemaFile("/usr/lib/dls2/dls_foxglove/FrameTransform.json");
        json jsonFrameSchema = json::parse(jsonFrameSchemaFile);

        const auto chanFrame = this->webserver_.addChannel({
            "frames",
            "json",
            jsonFrameSchema["title"],
            jsonFrameSchema.dump()
        });

        this->timer_flags_.insert(chanFrame);

        // Update scene
        std::ifstream jsonSceneSchemaFile("/usr/lib/dls2/dls_foxglove/SceneUpdate.json");
        json jsonSceneSchema = json::parse(jsonSceneSchemaFile);

        auto chanScene = this->webserver_.addChannel({
            "scene",
            "json",
            jsonSceneSchema["title"],
            jsonSceneSchema.dump()
        });

        // this->timer_flags_.insert(chanScene);

        // Handler for connection
        webserver_.setSubscribeHandler([&](ChannelId chanId) {
            std::unique_lock<std::mutex> lock(this->send_flags_mutex_);
            this->send_flags_.erase(chanId);
        });

        this->dds_link_.addReader(
            "blind_state_foxglove",
            dls::topics::low_level_estimation::blind_state,
            std::function<void(void *)>{[&, topic_name, chanFrame, chanScene, jsonPair, jsonFrameSchema, jsonSceneSchema, type_name](void *tuple)
            {
                std::unique_lock<std::mutex> lock(this->send_flags_mutex_);
                auto type_ = dds::get_type_registered_(type_name);

                nlohmann::json jsonMsg = jsonPair.second;

                dds::getDataToJson(topic_name, type_, (eprosima::fastrtps::types::DynamicData*) tuple, jsonMsg);

                std::ifstream jsonRobotFile("/usr/include/" + jsonMsg["robot_name"].get<std::string>() + "_description/foxglove/" + jsonMsg["robot_name"].get<std::string>() + ".json");

                // Apparently, it needs to store the value in a separate variable before filling the mcap_msg_.data field
                std::string serialized_json_scene = json::parse(jsonRobotFile).dump();

                if(this->send_flags_.find(chanScene) == this->send_flags_.end())
                {
                    webserver_.sendMessage(chanScene, nanosecondsSinceEpoch(), serialized_json_scene);
                    this->send_flags_.insert(chanScene);
                }

                // Fill the message to be sent on the MCAP channel
                if(mcap_ongoing_recording_)
                {
                    // Update the channel ID, add schema and channel to the writer, only if:
                    // - new topics are discovered
                    // - a new MCAP log is started after a previous one has been stopped
                    if (mcap_topics_channels_.find("scene") == mcap_topics_channels_.end())
                    {
                        mcap::Schema mcap_schema("foxglove.SceneUpdate", "jsonschema", jsonSceneSchema.dump());
                        mcap_writer_.addSchema(mcap_schema);

                        mcap::Channel mcap_channel("scene", "json", mcap_schema.id);
                        mcap_writer_.addChannel(mcap_channel);

                        mcap_topics_channels_.insert(std::pair<std::string, mcap::ChannelId>("scene", mcap_channel.id));

                        mcap_msg_.channelId = mcap_topics_channels_.find("scene")->second;
                        mcap_msg_.logTime = nanosecondsSinceEpoch(); // Required nanosecond timestamp
                        mcap_msg_.publishTime = mcap_msg_.logTime; // Set to logTime if not available
                        mcap_msg_.data = reinterpret_cast<const std::byte*>(serialized_json_scene.data());
                        mcap_msg_.dataSize = serialized_json_scene.size();

                        // In this case, we write a single message for the "scene" data, to not uselessly overload the message since the published values are alwasy the same
                        auto mcap_writer_output = mcap_writer_.write(mcap_msg_);

                        if (!mcap_writer_output.ok())
                        {
                            std::cerr << "Failed to write the MCAP scene message: " << mcap_writer_output.message << std::endl;
                        }
                    }
                }

                if(this->send_flags_.find(chanFrame) != this->send_flags_.end())
                    return;

                std::ifstream jsonFramesFile("/usr/include/" + jsonMsg["robot_name"].get<std::string>() + "_description/foxglove/" + jsonMsg["robot_name"].get<std::string>() + "_frames.json");

                json jsonFramesMsg = json::parse(jsonFramesFile);
                
                for(auto& frame : jsonFramesMsg["transforms"])
                {
                    if(frame["child_frame_id"] == "trunk")
                    {
                        frame["translation"]["x"] = jsonMsg["base_pos_world"][0];
                        frame["translation"]["y"] = jsonMsg["base_pos_world"][1];
                        frame["translation"]["z"] = jsonMsg["base_pos_world"][2];

                        frame["rotation"]["x"] = jsonMsg["base_ori_world"][0];
                        frame["rotation"]["y"] = jsonMsg["base_ori_world"][1];
                        frame["rotation"]["z"] = jsonMsg["base_ori_world"][2];
                        frame["rotation"]["w"] = jsonMsg["base_ori_world"][3];
                    }
                    else
                    {
                        int i = 0;
                        for(auto joint_name : jsonMsg["joint_name"])
                        {
                            if(joint_name == frame["child_frame_id"])
                            {
                                double a = frame["rotation"]["w"].get<double>();
                                double b = frame["rotation"]["x"].get<double>();
                                double c = frame["rotation"]["y"].get<double>();
                                double d = frame["rotation"]["z"].get<double>();

                                double e = cos(jsonMsg["joint_pos"][i].get<double>()/2);
                                double f = 0;
                                double g = 0;
                                double h = sin(jsonMsg["joint_pos"][i].get<double>()/2);

                                frame["rotation"]["x"] = b*e + a*f + c*h - d*g;
                                frame["rotation"]["y"] = a*g - b*h + c*e + d*f;
                                frame["rotation"]["z"] = a*h + b*g - c*f + d*e;
                                frame["rotation"]["w"] = a*e - b*f - c*g - d*h;
                                break;
                            }
                            i++;
                        }
                    }
                    webserver_.sendMessage(chanFrame, nanosecondsSinceEpoch(), frame.dump());
                }
                this->send_flags_.insert(chanFrame);

                // Fill the message to be sent on the MCAP channel
                if(mcap_ongoing_recording_)
                {
                    // Apparently, it needs to store the value in a separate variable before filling the mcap_msg_.data field
                    std::string serialized_json = jsonFramesMsg.dump();

                    // Update the channel ID, add schema and channel to the writer, only if:
                    // - new topics are discovered
                    // - a new MCAP log is started after a previous one has been stopped
                    if (mcap_topics_channels_.find("frames") == mcap_topics_channels_.end())
                    {
                        mcap::Schema mcap_schema("foxglove.FrameTransforms", "jsonschema", jsonFrameSchema.dump());
                        mcap_writer_.addSchema(mcap_schema);

                        mcap::Channel mcap_channel("frames", "json", mcap_schema.id);
                        mcap_writer_.addChannel(mcap_channel);

                        mcap_topics_channels_.insert(std::pair<std::string, mcap::ChannelId>("frames", mcap_channel.id));
                    }

                    mcap_msg_.channelId = mcap_topics_channels_.find("frames")->second;
                    mcap_msg_.logTime = nanosecondsSinceEpoch(); // Required nanosecond timestamp
                    mcap_msg_.publishTime = mcap_msg_.logTime; // Set to logTime if not available
                    mcap_msg_.data = reinterpret_cast<const std::byte*>(serialized_json.data());
                    mcap_msg_.dataSize = serialized_json.size();

                    auto mcap_writer_output = mcap_writer_.write(mcap_msg_);

                    if (!mcap_writer_output.ok())
                    {
                        std::cerr << "Failed to write the MCAP frame message: " << mcap_writer_output.message << std::endl;
                    }
                }
            }}
        );
    }
}