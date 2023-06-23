#ifndef MCAP_READER_UTILS_HPP
#define MCAP_READER_UTILS_HPP

#include "mcap/reader.hpp"
#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/msg_wrappers/signal_writer.hpp"
#include <nlohmann/json.hpp>

namespace dls
{
    class MCAPReaderUtils
    {
    public:
        MCAPReaderUtils();
        ~MCAPReaderUtils();

    	/**
		 * @brief Get the MCAP Reader object
		 * @return const mcap::McapReader&
		 */
        const mcap::McapReader& getMCAPReader();

    	/**
		 * @brief Verify if there is an ongoing playback of an MCAP log file
		 * @return bool
		 */
        bool isPlaybackOngoing();

    	/**
		 * @brief Initialize the MCAP reader to start the playback of an MCAP log file
		 * @return void
		 */
        void startPlayback(const std::string &mcap_log_file);

    	/**
		 * @brief Stop reading and publishing the MCAP log file data
		 * @return void
		 */
        void stopPlayback();

    	/**
		 * @brief Read an MCAP log file. It is possible to print the file content for debug
		 * @return void
		 */
		void readMCAPLog(bool print_mcap_log);

    	/**
		 * @brief Publish the contents of the read MCAP log file on the proper DDS topics
		 * @return void
		 */
		void publishMCAPLog(mcap::LinearMessageView::Iterator& mcap_iterator, nlohmann::json& parsed_message);

    private:
    	/**
		 * @brief Set the variable to change the status of the ongoing MCAP log file playback
		 * @return void
		 */
        void setPlaybackStatus(bool mcap_ongoing_playback);

    	/**
		 * @brief Close the active MCAP Writer, clear the MCAP Message fields and the map values 
		 * @return void
		 */
        void resetData();

		/**
		 * @brief MCAP reader 
		 */
		mcap::McapReader mcap_reader_{};

		/**
		 * @brief Used to verify if there is an ongoing playback of an MCAP log file
		 */
        bool mcap_ongoing_playback_{false};

		/**
		 * @brief Used to publish messages on DDS topics through the SignalWriter
		 */
		std::shared_ptr<dls::DDSParticipant> dds_participant_;

		/**
		 * @brief Used to compute the time passed after different MCAP messages, so as to set the sleep for synchronizing the published messages 
		 */
		std::chrono::steady_clock::time_point time_{};

		/**
		 * @brief Used to store the information regarding the previous MCAP message timestamp, needed for synchronizing the published messages
		 */
		mcap::Timestamp previous_timestamp_{};

		/**
		 * @brief Used to compute the amount of time that has to be waited before printing/publishing a new MCAP message
		 */
		mcap::Timestamp wait_for_publishing_{};
    };
} //namespace dls

#endif