#define MCAP_IMPLEMENTATION

#include "mcap_reader_utils.hpp"

namespace dls
{
    MCAPReaderUtils::MCAPReaderUtils()
        : mcap_reader_support_(std::make_shared<mcap_reader_support::MCAPReaderSupport>())
        , clogstream_("MCAPReaderUtils")
        {}

    MCAPReaderUtils::~MCAPReaderUtils()
    {
        resetData();
    }

    const mcap::McapReader& MCAPReaderUtils::getMCAPReader()
    {
        return mcap_reader_;
    }

    void MCAPReaderUtils::setPlaybackStatus(bool mcap_ongoing_playback)
    {
        mcap_ongoing_playback_ = mcap_ongoing_playback;
    }

    bool MCAPReaderUtils::isPlaybackOngoing()
    {
        return mcap_ongoing_playback_;
    }

    void MCAPReaderUtils::resetData()
    {
        if(isPlaybackOngoing())
        {
            mcap_reader_.close();
            setPlaybackStatus(false);
        }
    }

    void MCAPReaderUtils::startPlayback(const std::string &mcap_log_file)
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
                clogstream_ << "Start playback of an MCAP log file" << std::endl;

                setPlaybackStatus(true);

                readMCAPLog(false);

                clogstream_ << "End of MCAP log file" << std::endl;

                resetData();
            }
        }
        // Return if there is an error (e.g. the path to the MCAP file is not valid)
        catch(const std::string& error)
        {
            clogstream_ << error << std::endl;
            return;
        }
    }

    void MCAPReaderUtils::stopPlayback()
    {
        if(isPlaybackOngoing())
        {
            clogstream_ << "Stop playback of the MCAP log file" << std::endl;

            setPlaybackStatus(false);
        }
        else { clogstream_ << "No MCAP log playback ongoing" << std::endl; }
    }

    void MCAPReaderUtils::readMCAPLog(bool print_mcap_log)
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

            // Publish the MCAP message on DDS topic
            mcap_reader_support_->publishMessageOnTopic(it, parsed);

            // Print the MCAP log file content for debug
            if (print_mcap_log)
            {
                clogstream_ << it->message.sequence << ") " << it->channel->topic << "\t\t\t Time: " << it->message.logTime << std::endl;

                clogstream_ << "{" << std::endl;
                for (auto kv : parsed.items())
                {
                    clogstream_ << "\t" << kv.key() << ": " << kv.value() << std::endl;
                }
                clogstream_ << "}" << std::endl;
            }
        }
    }
}