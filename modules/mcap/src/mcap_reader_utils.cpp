#define MCAP_IMPLEMENTATION

#include "mcap_reader_utils.hpp"

namespace dls
{
    MCAPReaderUtils::MCAPReaderUtils()
        : mcap_reader_support_(std::make_shared<mcap_reader_support::MCAPReaderSupport>())
        , terminal_logger_("MCAPReaderUtils")
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
                terminal_logger_.info("Start playback of an MCAP log file");

                setPlaybackStatus(true);

                readMCAPLog(false);

                terminal_logger_.info("End of MCAP log file");

                resetData();
            }
        }
        // Return if there is an error (e.g. the path to the MCAP file is not valid)
        catch(const std::string& error)
        {
            terminal_logger_.error(error);
            return;
        }
    }

    void MCAPReaderUtils::stopPlayback()
    {
        if(isPlaybackOngoing())
        {
            terminal_logger_.info("Stop playback of the MCAP log file");

            setPlaybackStatus(false);
        }
        else { terminal_logger_.info("No MCAP log playback ongoing"); }
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
                terminal_logger_.info(it->message.sequence << ") " + it->channel->topic + "\t\t\t Time: " + it->message.logTime);

                terminal_logger_.info("{");
                for (auto kv : parsed.items())
                {
                    terminal_logger_.info("\t" + kv.key() + ": " + kv.value());
                }
                terminal_logger_.info("}");
            }
        }
    }
}