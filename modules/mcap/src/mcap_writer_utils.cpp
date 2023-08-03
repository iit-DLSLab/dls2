#define MCAP_IMPLEMENTATION

#include "mcap_writer_utils.hpp"

namespace dls
{
    MCAPWriterUtils::MCAPWriterUtils() : clogstream_("MCAPWriterUtils") {}

    MCAPWriterUtils::~MCAPWriterUtils()
    {
        resetData();
    }

    const mcap::McapWriter& MCAPWriterUtils::getMCAPWriter()
    {
        return mcap_writer_;
    }

    const mcap::Message& MCAPWriterUtils::getMCAPMessage()
    {
        return mcap_msg_;
    }

    const std::map<std::string, std::pair<mcap::ChannelId, int>>& MCAPWriterUtils::getTopicsMCAPDataMap()
    {
        return topics_and_mcap_data_;
    }

    void MCAPWriterUtils::setRecordingStatus(bool mcap_ongoing_recording)
    {
        mcap_ongoing_recording_ = mcap_ongoing_recording;
    }

    bool MCAPWriterUtils::isRecordingOngoing()
    {
        return mcap_ongoing_recording_;
    }

    void MCAPWriterUtils::resetData()
    {
        if(isRecordingOngoing())
        {
            mcap_writer_.close();
            mcap_msg_ = {};
            topics_and_mcap_data_.clear();
            sequence_counter_ = 0;
            scene_recorded_ = false;
            setRecordingStatus(false);
        }
    }

    void MCAPWriterUtils::startRecording(const std::string& timestamp)
    {
        // Close the current MCAP log file before recording, in case you did not stop the previous one
        if(isRecordingOngoing())
        {
            stopRecording();
        }

        // Initialize an MCAP writer with the "json" profile and write the MCAP file
        const auto mcap_writer_status = mcap_writer_.open("mcap_log_" + timestamp + ".mcap", mcap::McapWriterOptions(""));

        if (!mcap_writer_status.ok())
        {
          std::cerr << "Failed to open the MCAP file for writing: " << mcap_writer_status.message << std::endl;
        }
        else
        {
            clogstream_ << "Start recording an MCAP log" << std::endl;

            setRecordingStatus(true);
        }
    }

    void MCAPWriterUtils::stopRecording()
    {
        if(isRecordingOngoing())
        {
            clogstream_ << "Stop recording the MCAP log" << std::endl;

            resetData();

            setRecordingStatus(false);
        }
        else { clogstream_ << "No MCAP recording ongoing" << std::endl; }
    }

    void MCAPWriterUtils::writeMessage(const std::string& topic_name, const std::string_view& schema_encoding, 
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

    void MCAPWriterUtils::printTopicsMCAPDataMap()
    {
        if(topics_and_mcap_data_.size() > 0)
        {
            clogstream_ << "[MCAP channel] [Messages] Topic" << std::endl;
            clogstream_ << "----------------------" << std::endl;

            for(const auto& elem : topics_and_mcap_data_)
            {
                clogstream_ << "[" << elem.second.first << "] " << "[" << elem.second.second << "] " << elem.first << std::endl;
            }
        }
        else
        {
            clogstream_ << "The map with topics and associated MCAP channels is empty" << std::endl;
        }
    }
}