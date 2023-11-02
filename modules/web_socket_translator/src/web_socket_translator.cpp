#include "dls2/web_socket_translator/web_socket_translator.hpp"

namespace dls
{
    static uint64_t nanosecondsSinceEpoch() {
        return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::system_clock::now().time_since_epoch())
                        .count());
    }

	WebSocketTranslator::WebSocketTranslator(std::string& ID) 
		: Service(ID)
		, command_manager_(ID)
        , server_thread_(nullptr)
        , webserver_(8765, "webserver")
        , dds_participant_(std::make_shared<dls::DDSParticipant>("Web_Socket_Translator::monitor", dls::domains::signals, eprosima::fastrtps::rtps::DiscoveryProtocol_t::SUPER_CLIENT, false))
        , mcap_writer_utils_(std::make_shared<dls::MCAPWriterUtils>())
        , mcap_reader_utils_(std::make_shared<dls::MCAPReaderUtils>())
    {
        this->server_thread_ = std::make_shared<std::thread>(&WebSocketTranslator::serverFunc, this);
        dds_participant_->setTopicListener(this);

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

		// Start recording an MCAP log file
		command_manager.addCommand<>
		(
			"startRecording",
			"Start recording an MCAP log file",
			std::function<bool()>([&]() -> bool
			{
				record_mcap_log(true, get_current_time());
				return true;
			}),
			{{}},
			true
		);

		// Stop recording the MCAP log file
		command_manager.addCommand<>
		(
			"stopRecording",
			"Stop recording the MCAP log file",
			std::function<bool()>([&]()->bool
			{
					record_mcap_log(false);
					return true;
			}),
			{{}},
			true
		);

		// Start the playback of an MCAP log file
		command_manager.addCommand<>
		(
			"startPlaybackMCAP",
			"Start reading an MCAP log file and publish its data on DDS topics",
			std::function<bool(std::string mcap_log_file)>([&](const std::string &mcap_log_file)->bool
			{
					if(!mcap_log_file.empty())
					{
						playback_mcap_log(true, mcap_log_file);
						return true;
					}
					return false;
			}),
			{{}},
			true
		);

		// Start the playback of an MCAP log file
		command_manager.addCommand<>
		(
			"stopPlaybackMCAP",
			"Stop reading an MCAP log file and stop publishing its data on DDS topics",
			std::function<bool()>([&]()->bool
			{
					playback_mcap_log(false);
					return true;
			}),
			{{}},
			true
		);

		scout_sys << "SERVICE " + ID + " IS RUNNING" << std::endl;
	}

	WebSocketTranslator::~WebSocketTranslator()
	{
        server_thread_->join();

        webserver_.stop();

		scout_sys << "SERVICE " + this->getID() + " IS OFF" << std::endl;
	}

    void WebSocketTranslator::serverFunc()
    {
        webserver_.run();

        scout_sys << "#### Web Socket Translator Stopped #####" << std::endl;
    } 

    void WebSocketTranslator::on_topic_discovery(const std::string& topic_name, const std::string& type_name)
    {
        if (type_name.find("eprosima::fastdds::statistics::") != std::string::npos)
            return;

        scout_sys << "Topic discovered: " << topic_name << " [ " << type_name << " ]" << std::endl;

        auto type_ = dds::get_type_registered_(type_name);

        auto jsonPair = dds::createJsonSchema(topic_name, type_);

        auto channel = this->webserver_.addChannel({
            topic_name,
            "json",
            jsonPair.first["title"],
            jsonPair.first.dump()
        });

        this->timer_flags_.insert(channel);

        this->dds_participant_->addReader(
                topic_name,
                dls::topicType({topic_name, eprosima::fastdds::dds::TypeSupport(new eprosima::fastrtps::types::DynamicPubSubType(type_))}),
                std::function<void(void *)>{[&, topic_name, type_name, jsonPair, channel](void *tuple)
                {
                    std::unique_lock<std::mutex> lock(this->send_flags_mutex_);
                    auto type_ = dds::get_type_registered_(type_name);

                    if(this->send_flags_.find(channel) != this->send_flags_.end())
                        return;

                    nlohmann::json json_pair_first = jsonPair.first;
                    nlohmann::json json_pair_second = jsonPair.second;

                    dds::getDataToJson(topic_name, type_, (eprosima::fastrtps::types::DynamicData*) tuple, json_pair_second);

                    webserver_.sendMessage(channel, nanosecondsSinceEpoch(), json_pair_second.dump());
                    this->send_flags_.insert(channel);

                    // Write an MCAP message with the topic data
                    if(mcap_writer_utils_->isRecordingOngoing())
                    {
                        const auto schema_data{json_pair_first.dump()};
                        const auto message_data{json_pair_second.dump()};

                        mcap_writer_utils_->writeMessage(topic_name, type_name, schema_data, message_data, nanosecondsSinceEpoch());
                    }
                }}
        );

        if(topic_name == "blind_state")
        {
            // Create frames
            std::ifstream jsonFrameSchemaFile("/usr/lib/dls2/services/web_socket_translator/FrameTransform.json");
            nlohmann::json jsonFrameSchema = nlohmann::json::parse(jsonFrameSchemaFile);

            const auto chanFrame = this->webserver_.addChannel({
                "frames",
                "json",
                jsonFrameSchema["title"],
                jsonFrameSchema.dump()
            });

            this->timer_flags_.insert(chanFrame);

            // Update scene
            std::ifstream jsonSceneSchemaFile("/usr/lib/dls2/services/web_socket_translator/SceneUpdate.json");
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

            this->dds_participant_->addReader(
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
                    if(mcap_writer_utils_->isRecordingOngoing())
                    {
                        const auto schema_data{jsonSceneSchema.dump()};
                        const auto message_data{serialized_json_scene};

                        mcap_writer_utils_->writeMessage("scene", "foxglove.SceneUpdate", schema_data, message_data, nanosecondsSinceEpoch());
                    }

                    if(this->send_flags_.find(chanFrame) != this->send_flags_.end())
                        return;

                    std::ifstream jsonFramesFile("/usr/include/" + jsonMsg["robot_name"].get<std::string>() + "_description/foxglove/" + jsonMsg["robot_name"].get<std::string>() + "_frames.json");

                    nlohmann::json jsonFramesMsg = nlohmann::json::parse(jsonFramesFile);
                    
                    for(auto& frame : jsonFramesMsg["transforms"])
                    {
                        int i = 0;
                        for(auto joint_name : jsonMsg["joints_name"])
                        {
                            if(joint_name == frame["child_frame_id"])
                            {
                                double a = frame["rotation"]["w"].get<double>();
                                double b = frame["rotation"]["x"].get<double>();
                                double c = frame["rotation"]["y"].get<double>();
                                double d = frame["rotation"]["z"].get<double>();

                                double e = cos(jsonMsg["joints_position"][i].get<double>()/2);
                                double f = 0;
                                double g = 0;
                                double h = sin(jsonMsg["joints_position"][i].get<double>()/2);

                                frame["rotation"]["x"] = b*e + a*f + c*h - d*g;
                                frame["rotation"]["y"] = a*g - b*h + c*e + d*f;
                                frame["rotation"]["z"] = a*h + b*g - c*f + d*e;
                                frame["rotation"]["w"] = a*e - b*f - c*g - d*h;
                                webserver_.sendMessage(chanFrame, nanosecondsSinceEpoch(), frame.dump());
                                break;
                            }
                            i++;
                        }
                    }
                    this->send_flags_.insert(chanFrame);

                    // Write an MCAP message with the "frame" data
                    if(mcap_writer_utils_->isRecordingOngoing())
                    {
                        const auto schema_data{jsonFrameSchema.dump()};
                        const auto message_data{jsonFramesMsg.dump()};

                        mcap_writer_utils_->writeMessage("frames", "foxglove.FrameTransforms", schema_data, message_data, nanosecondsSinceEpoch());
                    }
                }}
            );
        }
        if(topic_name == "base_state")
        {
            // Create frames
            std::ifstream jsonFrameSchemaFile("/usr/lib/dls2/services/web_socket_translator/FrameTransform.json");
            nlohmann::json jsonFrameSchema = nlohmann::json::parse(jsonFrameSchemaFile);

            const auto chanFrame = this->webserver_.addChannel({
                "frames_base_state",
                "json",
                jsonFrameSchema["title"],
                jsonFrameSchema.dump()
            });

            this->timer_flags_.insert(chanFrame);

            // Update scene
            std::ifstream jsonSceneSchemaFile("/usr/lib/dls2/services/web_socket_translator/SceneUpdate.json");
            nlohmann::json jsonSceneSchema = nlohmann::json::parse(jsonSceneSchemaFile);

            auto chanScene = this->webserver_.addChannel({
                "scene_base_state",
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

            this->dds_participant_->addReader(
                "base_state_foxglove",
                dls::topics::high_level_estimation::base_state,
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
                    if(mcap_writer_utils_->isRecordingOngoing())
                    {
                        const auto schema_data{jsonSceneSchema.dump()};
                        const auto message_data{serialized_json_scene};

                        mcap_writer_utils_->writeMessage("scene", "foxglove.SceneUpdate", schema_data, message_data, nanosecondsSinceEpoch());
                    }

                    if(this->send_flags_.find(chanFrame) != this->send_flags_.end())
                        return;

                    std::ifstream jsonFramesFile("/usr/include/" + jsonMsg["robot_name"].get<std::string>() + "_description/foxglove/" + jsonMsg["robot_name"].get<std::string>() + "_frames.json");

                    nlohmann::json jsonFramesMsg = nlohmann::json::parse(jsonFramesFile);
                    
                    for(auto& frame : jsonFramesMsg["transforms"])
                    {
                        if(frame["child_frame_id"] == "trunk")
                        {
                            frame["translation"]["x"] = jsonMsg["position"][0];
                            frame["translation"]["y"] = jsonMsg["position"][1];
                            frame["translation"]["z"] = jsonMsg["position"][2];

                            frame["rotation"]["x"] = jsonMsg["orientation"][0];
                            frame["rotation"]["y"] = jsonMsg["orientation"][1];
                            frame["rotation"]["z"] = jsonMsg["orientation"][2];
                            frame["rotation"]["w"] = jsonMsg["orientation"][3];
                            webserver_.sendMessage(chanFrame, nanosecondsSinceEpoch(), frame.dump());
                            break;
                        }
                    }
                    this->send_flags_.insert(chanFrame);

                    // Write an MCAP message with the "frame" data
                    if(mcap_writer_utils_->isRecordingOngoing())
                    {
                        const auto schema_data{jsonFrameSchema.dump()};
                        const auto message_data{jsonFramesMsg.dump()};

                        mcap_writer_utils_->writeMessage("frames", "foxglove.FrameTransforms", schema_data, message_data, nanosecondsSinceEpoch());
                    }
                }}
            );
        }
    }

    void WebSocketTranslator::record_mcap_log(bool record_mcap, const std::string& timestamp)
    {
        // Start recording a new MCAP log file
        if(record_mcap)
        {
            mcap_writer_utils_->startRecording(timestamp);
        }
        // Stop recording the current MCAP log file
        else
        {
            mcap_writer_utils_->stopRecording();
        }
    }

    void WebSocketTranslator::playback_mcap_log(bool playback_mcap, const std::string &mcap_log_file)
    {
        // Start the playback of an MCAP log file
        if(playback_mcap)
        {
            mcap_reader_utils_->startPlayback(mcap_log_file);
        }
        // Stop the playback of the MCAP log file
        else
        {
            mcap_reader_utils_->stopPlayback();
        }
    }

	// the class factories
    extern "C" WebSocketTranslator* create(std::string ID) 
    {
        return new WebSocketTranslator(ID);
    }

    extern "C" void destroy(WebSocketTranslator* p) 
    {
        delete p;
    }
}