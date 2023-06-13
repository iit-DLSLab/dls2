#define MCAP_IMPLEMENTATION

#include "mcap_utils.hpp"

namespace dls
{
    MCAPUtils::MCAPUtils() 
        : dds_participant_(std::make_shared<dls::DDSParticipant>("MCAPUtils::signals", dls::domains::signals))
    {}

    MCAPUtils::~MCAPUtils()
    {
        resetData();
    }

    const mcap::McapWriter& MCAPUtils::getMCAPWriter()
    {
        return mcap_writer_;
    }

    const mcap::McapReader& MCAPUtils::getMCAPReader()
    {
        return mcap_reader_;
    }

    const mcap::Message& MCAPUtils::getMCAPMessage()
    {
        return mcap_msg_;
    }

    const std::map<std::string, std::pair<mcap::ChannelId, int>>& MCAPUtils::getTopicsMCAPDataMap()
    {
        return topics_and_mcap_data_;
    }

    void MCAPUtils::setRecordingStatus(bool mcap_ongoing_recording)
    {
        mcap_ongoing_recording_ = mcap_ongoing_recording;
    }

    void MCAPUtils::setPlaybackStatus(bool mcap_ongoing_playback)
    {
        mcap_ongoing_playback_ = mcap_ongoing_playback;
    }

    bool MCAPUtils::isRecordingOngoing()
    {
        return mcap_ongoing_recording_;
    }

    bool MCAPUtils::isPlaybackOngoing()
    {
        return mcap_ongoing_playback_;
    }

    void MCAPUtils::resetData()
    {
        if(isRecordingOngoing())
        {
            mcap_writer_.close();
            mcap_msg_ = {};
            topics_and_mcap_data_.clear();
            sequence_counter_ = 0;
            mcap_ongoing_recording_ = false;
            scene_recorded_ = false;
        }
        if(isPlaybackOngoing())
        {
            mcap_reader_.close();
            mcap_ongoing_playback_ = false;
            //begin_ = 0;
        }
    }

    void MCAPUtils::startRecording(const std::string& timestamp)
    {
        // Close the current MCAP log file before recording, in case you did not stop the previous one
        if(isRecordingOngoing())
        {
            stopRecording();
        }

        // Initialize an MCAP writer with the "json" profile and write the MCAP file
        const auto mcap_writer_status = mcap_writer_.open("mcap_log_" + timestamp + ".mcap", mcap::McapWriterOptions("json"));

        if (!mcap_writer_status.ok())
        {
          std::cerr << "Failed to open the MCAP file for writing: " << mcap_writer_status.message << std::endl;
        }
        else
        {
            std::cout << "Start recording an MCAP log" << std::endl;
        }

        setRecordingStatus(true);
    }

    void MCAPUtils::stopRecording()
    {
        if(isRecordingOngoing())
        {
            std::cout << "Stop recording the MCAP log" << std::endl;

            resetData();
        }

        setRecordingStatus(false);
    }

    void MCAPUtils::startPlayback(const std::string &mcap_log_file)
    {
        // Close the previous MCAP reader before running a playback again
        if(isPlaybackOngoing())
        {
            stopPlayback();
        }

        try
        {
            // Initialize an MCAP reader with the MCAP log file passed as parameter
            const auto mcap_reader_status = mcap_reader_.open(mcap_log_file);
            if (!mcap_reader_status.ok())
            {
                throw std::string("Failed to open " + mcap_log_file + " for reading. Error: " + mcap_reader_status.message);
            }
            else
            {
                std::cout << "Start playback of an MCAP log file" << std::endl;

                setPlaybackStatus(true);

                readMCAPLog(true);
            }
        }
        // Return if there is an error (e.g. the path to the MCAP file is not valid)
        catch(const std::string& error)
        {
            std::cout << error << std::endl;
            return;
        }
    }

    void MCAPUtils::stopPlayback()
    {
        if(isPlaybackOngoing())
        {
            std::cout << "Stop playback of the MCAP log file" << std::endl;

            resetData();
        }

        setPlaybackStatus(false);
    }

    void MCAPUtils::writeMessage(const std::string& topic_name, const std::string_view& schema_encoding, 
                                 const std::string& schema_data, const std::string& message_data, const uint64_t msg_log_time)
    {
        // Update the channel ID, add schema and channel to the writer, only if:
        // - new topics are discovered
        // - a new MCAP log is started after a previous one has been stopped
        if (topics_and_mcap_data_.find(topic_name) == topics_and_mcap_data_.end())
        {
            mcap::Schema mcap_schema(schema_encoding, "jsonschema", schema_data);
            mcap_writer_.addSchema(mcap_schema);

            mcap::Channel mcap_channel(topic_name, "json", mcap_schema.id);
            mcap_writer_.addChannel(mcap_channel);

            topics_and_mcap_data_.insert(std::make_pair(topic_name, std::make_pair(mcap_channel.id, 0)));
        }

        // Write the MCAP message for every topic, but just once for "scene" (since it publishes always the same data)
        if((topic_name != "scene") or (!scene_recorded_))
        {
            sequence_counter_++;

            mcap_msg_.channelId = topics_and_mcap_data_.find(topic_name)->second.first;
            mcap_msg_.logTime = msg_log_time; // Required nanosecond timestamp
            mcap_msg_.publishTime = mcap_msg_.logTime; // Set to logTime if not available
            mcap_msg_.sequence = sequence_counter_;
            mcap_msg_.data = reinterpret_cast<const std::byte*>(message_data.data());
            mcap_msg_.dataSize = message_data.size();

            auto mcap_writer_output = mcap_writer_.write(mcap_msg_);

            if (!mcap_writer_output.ok())
            {
                std::cerr << "Failed to write the MCAP message for topic '" << topic_name << "'. Error: " << mcap_writer_output.message << std::endl;
            }

            if(topic_name == "scene")
            {
                scene_recorded_ = true;
            }

            ((topics_and_mcap_data_.find(topic_name))->second.second)++;
        }
    }

    void MCAPUtils::readMCAPLog(bool print_mcap_log)
    {
        auto message_view = mcap_reader_.readMessages();

        for (auto it = message_view.begin(); it != message_view.end(); it++)
        {
            if(!isPlaybackOngoing())
            {
                return;
            }

            // Skip any non-json-encoded messages
            if (it->channel->messageEncoding != "json")
            {
                continue;
            }

            std::string_view asString(reinterpret_cast<const char*>(it->message.data), it->message.dataSize);

            auto parsed = nlohmann::json::parse(asString, nullptr, false);

            if (parsed.is_discarded())
            {
                std::cerr << "Failed to parse JSON: " << asString << std::endl;
            }

            if (!parsed.is_object())
            {
                std::cerr << "Unexpected non-object message: " << asString << std::endl;
            }

            publishMCAPLog(it, parsed);

            previous_timestamp_ = it->message.logTime;

            // Print the MCAP log file content for debug
            if (print_mcap_log)
            {
                std::cout << it->message.sequence << ") " << it->channel->topic << "\t\t\t Time: " << it->message.logTime << std::endl;

                std::cout << "{" << std::endl;
                for (auto kv : parsed.items())
                {
                    std::cout << "\t" << kv.key() << ": " << kv.value() << std::endl;
                }
                std::cout << "}" << std::endl;
            }
        }

        stopPlayback();
    }

    void MCAPUtils::publishMCAPLog(mcap::LinearMessageView::Iterator& mcap_iterator, nlohmann::json& parsed_message)
    {
        // Temporarily duplicated codes inside this function (e.g. topics publication, clocks for sleep)

        //if(mcap_iterator->channel->topic == "topic")
        //{
        //    dls::topicType;
        //    writer;
        //    for loop;
        //    sleep;
        //    publish;
        //}

        if(mcap_iterator->channel->topic == "imu:trunk_imu")
        {
            dls::topicType imu_topic_gt = dls::topicType(dls::topics::low_level_estimation::imu.first + ":trunk_imu", new ImuMsgPubSubType());

            mcap_playback_topic_writer_ = std::make_shared<SignalWriter<Imu>>(dds_participant_, imu_topic_gt, std::make_shared<Imu>());

            for (auto item : parsed_message.items())
            {
                if(item.key() == "angular_velocity")
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*mcap_playback_topic_writer_)->angular_velocity(i) = value.value();
                        i++;
                    }
                }
                if(item.key() == "angular_velocity_covariance")
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*mcap_playback_topic_writer_)->angular_velocity_covariance(i) = value.value();
                        i++;
                    }
                }
                if(item.key() == "linear_acceleration")
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*mcap_playback_topic_writer_)->linear_acceleration(i) = value.value();
                        i++;
                    }
                }
                if(item.key() == "linear_acceleration_covariance")
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*mcap_playback_topic_writer_)->linear_acceleration_covariance(i) = value.value();
                        i++;
                    }
                }
                if(item.key() == "orientation")
                {
                    std::vector<double> values{};

                    for (const auto& value : item.value().items())
                    {
                        values.push_back(value.value());
                    }

                    (*mcap_playback_topic_writer_)->orientation.x() = values.at(0);
                    (*mcap_playback_topic_writer_)->orientation.y() = values.at(1);
                    (*mcap_playback_topic_writer_)->orientation.z() = values.at(2);
                    (*mcap_playback_topic_writer_)->orientation.w() = values.at(3);
                }
                if(item.key() == "orientation_covariance")
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*mcap_playback_topic_writer_)->orientation_covariance(i) = value.value();
                        i++;
                    }
                }
                if(item.key() == "time")
                {
                    (*mcap_playback_topic_writer_)->time = item.value();
                }
                if(item.key() == "frame_id")
                {
                    (*mcap_playback_topic_writer_)->frame_id = item.value();
                }
            }

            if(mcap_iterator->message.sequence > 1)
            {
                auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - begin_).count();

                // You consider two elements for slowing down the print of a new message "M_i":
                // (1) "T_M_i - T_M_i-1". That is the difference of timestamps (nanoseconds) from message "i" and message "i-1"
                // (2) the elapsed time (nanoseconds) from the beginning of the for loop to the moment in which you print the message
                // You get "W_P = (1) - (2)" (nanoseconds) and publish immediately or sleep for "W_P" (if greater than zero) nanoseconds before printing the message  
                wait_for_publishing_ = (mcap_iterator->message.logTime - previous_timestamp_) - elapsed_time;

                if (wait_for_publishing_ > 0) {std::this_thread::sleep_for(std::chrono::nanoseconds(wait_for_publishing_));}
            }
            
            mcap_playback_topic_writer_->publish();
        }

        /*************/

        else
        {
            if(mcap_iterator->message.sequence > 1)
            {
                auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - begin_).count();

                // You consider two elements for slowing down the print of a new message "M_i":
                // (1) "T_M_i - T_M_i-1". That is the difference of timestamps (nanoseconds) from message "i" and message "i-1"
                // (2) the elapsed time (nanoseconds) from the beginning of the for loop to the moment in which you print the message
                // You get "W_P = (1) - (2)" (nanoseconds) and publish immediately or sleep for "W_P" (if greater than zero) nanoseconds before printing the message  
                wait_for_publishing_ = (mcap_iterator->message.logTime - previous_timestamp_) - elapsed_time;

                if (wait_for_publishing_ > 0) {std::this_thread::sleep_for(std::chrono::nanoseconds(wait_for_publishing_));}
            }
        }

        begin_ = std::chrono::steady_clock::now();
    }

    void MCAPUtils::printTopicsMCAPDataMap()
    {
        if(topics_and_mcap_data_.size() > 0)
        {
            std::cout << "\n[MCAP channel] [Messages] Topic" << std::endl;
            std::cout << "----------------------" << std::endl;

            for(const auto& elem : topics_and_mcap_data_)
            {
                std::cout << "[" << elem.second.first << "] " << "[" << elem.second.second << "] " << elem.first << std::endl;
            }
        }
        else
        {
            std::cout << "The map with topics and associated MCAP channels is empty" << std::endl;
        }
    }
}