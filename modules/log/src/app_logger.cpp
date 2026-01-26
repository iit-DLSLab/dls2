#include "dls2/log/app_logger.hpp"

using namespace dls::logging;

AppLogger::AppLogger(const std::string& ID, bool enable_log_terminal, bool enable_event_notify, bool enable_file_log)
: terminal_logger_(ID)
, event_notifier_(ID)
, enable_log_terminal_(enable_log_terminal)
, enable_event_notify_(enable_event_notify)
, enable_file_log_(enable_file_log)
{
	char * val;
	val = getenv("DLS_LOG_FOLDER_PATH");
	std::string log_path = "";
	if (val != NULL) {
		log_path = val;
	}
	else{
		log_path = "./logs/";
	}
	file_logger_ = std::make_unique<FileLogger>(log_path + ID + ".log");
}

void AppLogger::info(const std::string& s) {
	if(enable_event_notify_){
		event_notifier_.notify(EventID::NOMINAL_OPERATION, EventSeverity::INFO, s);
	}

	std::string line;
	if(enable_log_terminal_ || enable_file_log_){
		terminal_logger_.info(s, enable_log_terminal_, enable_file_log_ ? &line : nullptr);
	}
	if(enable_file_log_ && file_logger_ != nullptr && file_logger_->is_open() && !line.empty()){
		file_logger_->log(line);
	}
}

void AppLogger::debug(const std::string& s) {
	if(enable_event_notify_){
		event_notifier_.notify(EventID::NOMINAL_OPERATION, EventSeverity::INFO, s);
	}

	std::string line;
	if(enable_log_terminal_ || enable_file_log_){
		terminal_logger_.debug(s, enable_log_terminal_, enable_file_log_ ? &line : nullptr);
	}

	if(enable_file_log_ && file_logger_ != nullptr && file_logger_->is_open() && !line.empty()){
		file_logger_->log(line);
	}
}

void AppLogger::warning(const std::string& s, const EventID& event_id) {
	if(enable_event_notify_){
		event_notifier_.notify(event_id, EventSeverity::WARNING, s);
	}

	std::string line;
	if(enable_log_terminal_ || enable_file_log_){
		terminal_logger_.warning(s, enable_log_terminal_, enable_file_log_ ? &line : nullptr);
	}

	if(enable_file_log_ && file_logger_ != nullptr && file_logger_->is_open() && !line.empty()){
		file_logger_->log(line);
	}
}

void AppLogger::error(const std::string& s, const EventID& event_id) {
	if(enable_event_notify_){
		event_notifier_.notify(event_id, EventSeverity::ERROR, s);
	}

	std::string line;
	if(enable_log_terminal_ || enable_file_log_){
		terminal_logger_.error(s, enable_log_terminal_, enable_file_log_ ? &line : nullptr);
	}

	if(enable_file_log_ && file_logger_ != nullptr && file_logger_->is_open() && !line.empty()){
		file_logger_->log(line);
	}
}

void AppLogger::fatal(const std::string& s, const EventID& event_id) {
	if(enable_event_notify_){
		event_notifier_.notify(event_id, EventSeverity::FATAL, s);
	}

	std::string line;
	if(enable_log_terminal_ || enable_file_log_){
		terminal_logger_.fatal(s, enable_log_terminal_, enable_file_log_ ? &line : nullptr);
	}

	if(enable_file_log_ && file_logger_ != nullptr && file_logger_->is_open() && !line.empty()){
		file_logger_->log(line);
	}
}