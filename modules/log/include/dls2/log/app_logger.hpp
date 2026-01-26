#pragma once

#include "dls2/log/terminal_logger.hpp"
#include "dls2/log/event_logger.hpp"
#include "dls2/log/file_logger.hpp"

namespace dls
{
	namespace logging
	{   
		class AppLogger {
			public:
				explicit AppLogger(const std::string& ID, 
								   bool enable_log_terminal = true, 
								   bool enable_event_notify = false, 
								   bool enable_file_log = false);

				void info(const std::string& s);
				void debug(const std::string& s);
				void warning(const std::string& s, const EventID& event_id = EventID::GENERAL_WARNING);
				void error(const std::string& s, const EventID& event_id = EventID::GENERAL_ERROR);
				void fatal(const std::string& s, const EventID& event_id = EventID::GENERAL_ERROR);

			private:

				void write_file_if_enabled_(const std::string& line);
				
				TerminalLogger terminal_logger_;
				EventNotifier event_notifier_; // TODO: put robust event notifier
				std::unique_ptr<FileLogger> file_logger_ { nullptr };

				bool enable_log_terminal_;
				bool enable_event_notify_;
				bool enable_file_log_;
			};

        } // namespace logging
} // namespace dls