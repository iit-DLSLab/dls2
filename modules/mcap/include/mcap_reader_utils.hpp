#ifndef MCAP_READER_UTILS_HPP
#define MCAP_READER_UTILS_HPP

#include "mcap/reader.hpp"
#include "mcap_support/mcap_reader_support.hpp"
#include "dls2/log/terminal_logger.hpp"
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
		std::shared_ptr<mcap_reader_support::MCAPReaderSupport> mcap_reader_support_;

		/**
		 * @brief Used to verify if there is an ongoing playback of an MCAP log file
		 */
        bool mcap_ongoing_playback_{false};

		/**
		 * @brief Used to print information in Log layer
		 */
		dls::logging::TerminalLogger terminal_logger_;
    };
} //namespace dls

#endif