#include <foxglove/foxserver.hpp>
#include <fstream>
#include <Eigen/Dense>

using namespace boost;

namespace dls
{
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
        server_thread_->join();

        webserver_.stop();
    }

    void FoxServer::record_mcap_log(bool record_mcap, const std::string& timestamp)
    {
        // Start recording a new MCAP log file
        if(record_mcap)
        {
            mcap_utils_.startRecording(timestamp);
        }
        // Stop recording the current MCAP log file
        else
        {
            mcap_utils_.stopRecording();
        }
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

                    // Write an MCAP message with the topic data
                    if(mcap_utils_.isRecordingOngoing())
                    {
                        const auto schema_data{jsonPair.first.dump()};
                        const auto message_data{jsonPair.second.dump()};

                        mcap_utils_.writeMessage(topic_name, type_name, schema_data, message_data, nanosecondsSinceEpoch());
                    }
                }}
        );

        if(topic_name == "blind_state")
        {
            // Create frames
            std::ifstream jsonFrameSchemaFile("/usr/lib/dls2/dls_foxglove/FrameTransform.json");
            nlohmann::json jsonFrameSchema = nlohmann::json::parse(jsonFrameSchemaFile);

            const auto chanFrame = this->webserver_.addChannel({
                "frames",
                "json",
                jsonFrameSchema["title"],
                jsonFrameSchema.dump()
            });

            this->timer_flags_.insert(chanFrame);

            // Update scene
            std::ifstream jsonSceneSchemaFile("/usr/lib/dls2/dls_foxglove/SceneUpdate.json");
            nlohmann::json jsonSceneSchema = nlohmann::json::parse(jsonSceneSchemaFile);

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

                    // Apparently, it needs to store the value in a separate variable for filling the MCAP message
                    std::string serialized_json_scene = nlohmann::json::parse(jsonRobotFile).dump();

                    if(this->send_flags_.find(chanScene) == this->send_flags_.end())
                    {
                        webserver_.sendMessage(chanScene, nanosecondsSinceEpoch(), serialized_json_scene);
                        this->send_flags_.insert(chanScene);
                    }

                    // Write an MCAP message with the "scene" data
                    if(mcap_utils_.isRecordingOngoing())
                    {
                        const auto schema_data{jsonSceneSchema.dump()};
                        const auto message_data{serialized_json_scene};

                        mcap_utils_.writeMessage("scene", "foxglove.SceneUpdate", schema_data, message_data, nanosecondsSinceEpoch());
                    }

                    if(this->send_flags_.find(chanFrame) != this->send_flags_.end())
                        return;

                    std::ifstream jsonFramesFile("/usr/include/" + jsonMsg["robot_name"].get<std::string>() + "_description/foxglove/" + jsonMsg["robot_name"].get<std::string>() + "_frames.json");

                    nlohmann::json jsonFramesMsg = nlohmann::json::parse(jsonFramesFile);
                    
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

                    // Write an MCAP message with the "frame" data
                    if(mcap_utils_.isRecordingOngoing())
                    {
                        const auto schema_data{jsonFrameSchema.dump()};
                        const auto message_data{jsonFramesMsg.dump()};

                        mcap_utils_.writeMessage("frames", "foxglove.FrameTransforms", schema_data, message_data, nanosecondsSinceEpoch());
                    }
                }}
            );
        }
    }
}