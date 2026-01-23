#ifndef MCAP_WRITER_UTILS_HPP
#define MCAP_WRITER_UTILS_HPP

#include "mcap/writer.hpp"
#include "dls2/log/terminal_logger.hpp"
#include <nlohmann/json.hpp>
#include <map>

namespace dls
{
    class MCAPWriterUtils
    {
    public:
        MCAPWriterUtils();
        ~MCAPWriterUtils();

    	/**
		 * @brief Get the MCAP Writer object
		 * @return const mcap::McapWriter&
		 */
        const mcap::McapWriter& getMCAPWriter();

    	/**
		 * @brief Get the MCAP Message object
		 * @return mcap::Message&
		 */
        const mcap::Message& getMCAPMessage();

    	/**
		 * @brief Get the map with topics and associated MCAP channels
		 * @return std::map<std::string, std::pair<mcap::ChannelId, int>>&
		 */
        const std::map<std::string, std::pair<mcap::ChannelId, int>>& getTopicsMCAPDataMap();

    	/**
		 * @brief Verify if there is an ongoing recording of an MCAP log file
		 * @return bool
		 */
        bool isRecordingOngoing();

    	/**
		 * @brief Initialize the MCAP writer to start recording an MCAP log file
		 * @return void
		 */
        void startRecording(const std::string& timestamp);

    	/**
		 * @brief Stop recording the current MCAP log file
		 * @return void
		 */
        void stopRecording();

    	/**
		 * @brief Write the MCAP Message with topic information
		 * @return void
		 */
        void writeMessage(const std::string& topic_name, const std::string_view& schema_encoding, 
                          const std::string& schema_data, const std::string& message_data, const uint64_t msg_log_time);

    	/**
		 * @brief Print the the map with topics and associated MCAP channels 
		 * @return void
		 */
        void printTopicsMCAPDataMap();

    private:
    	/**
		 * @brief Set the variable to change the status of the ongoing MCAP log file recording
		 * @return void
		 */
        void setRecordingStatus(bool mcap_ongoing_recording);

    	/**
		 * @brief Close the active MCAP Writer, clear the MCAP Message fields and the map values 
		 * @return void
		 */
        void resetData();

		/**
		 * @brief MCAP writer 
		 */
        mcap::McapWriter mcap_writer_{};

		/**
		 * @brief MCAP message 
		 */
        mcap::Message mcap_msg_{};

		/**
		 * @brief Map with topics (Key) and associated MCAP channels (Value 1) and number of published messages (Value 2). 
		 * Used for writing new messages and for debug (Value 2 only used for debug) 
		 */
        std::map<std::string, std::pair<mcap::ChannelId, int>> topics_and_mcap_data_{};

		/**
		 * @brief Used to verify if there is an ongoing recording of an MCAP log file
		 */
        bool mcap_ongoing_recording_{false};

		/**
		 * @brief Used to verify if "scene" data have been recorded (to record it only once for each MCAP log file)
		 */
		bool scene_recorded_{false};

		/**
		 * @brief Used to valorize the sequence field of MCAP message and so, enumerating the messages in an MCAP log file
		 */
		int sequence_counter_{};

		/**
		 * @brief Used to print information in Log layer
		 */
		dls::logging::TerminalLogger terminal_logger_;
    };
} //namespace dls

#endif