#define MCAP_IMPLEMENTATION

#include "mcap_utils.hpp"

namespace dls
{
    MCAPUtils::MCAPUtils(){}

    MCAPUtils::~MCAPUtils()
    {
        resetData();
    }

    const mcap::McapWriter& MCAPUtils::getMCAPWriter()
    {
        return mcap_writer_;
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

    bool MCAPUtils::isRecordingOngoing()
    {
        return mcap_ongoing_recording_;
    }

    void MCAPUtils::startRecording(const std::string& timestamp)
    {
        // Close the current MCAP log file before recording, in case you did not stop the previous one
        if(isRecordingOngoing())
        {
            resetData();
        }

        std::cout << "Start recording an MCAP log" << std::endl;

        // Initialize an MCAP writer with the "json" profile and write the MCAP file
        auto mcap_writer_status = mcap_writer_.open("mcap_log_" + timestamp + ".mcap", mcap::McapWriterOptions("json"));

        if (!mcap_writer_status.ok())
        {
          std::cerr << "Failed to open the MCAP file for writing: " << mcap_writer_status.message << std::endl;
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
            mcap_msg_.channelId = topics_and_mcap_data_.find(topic_name)->second.first;
            mcap_msg_.logTime = msg_log_time; // Required nanosecond timestamp
            mcap_msg_.publishTime = mcap_msg_.logTime; // Set to logTime if not available
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

    void MCAPUtils::resetData()
    {
        mcap_writer_.close();
        mcap_msg_ = {};
        topics_and_mcap_data_.clear();
        mcap_ongoing_recording_ = false;
        scene_recorded_ = false;
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