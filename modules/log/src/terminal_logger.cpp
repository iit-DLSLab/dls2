#include "dls2/log/terminal_logger.hpp"

using namespace dls::logging;

TerminalLogger::TerminalLogger(const std::string& ID)
	: id_(ID)
{}

void TerminalLogger::info(const std::string& s, bool enable /* = true */, std::string* out_line /* = nullptr */)    {
	log(Level::INFO,    s, enable, out_line);
}
void TerminalLogger::debug(const std::string& s, bool enable /* = true */, std::string* out_line /* = nullptr */)    {
	log(Level::DEBUG,    s, enable, out_line);
}
void TerminalLogger::warning(const std::string& s, bool enable /* = true */, std::string* out_line /* = nullptr */) {
	log(Level::WARNING, s, enable, out_line);
}
void TerminalLogger::error(const std::string& s, bool enable /* = true */, std::string* out_line /* = nullptr */)   {
	log(Level::ERROR,   s, enable, out_line);
}
void TerminalLogger::fatal(const std::string& s, bool enable /* = true */, std::string* out_line /* = nullptr */)   {
	log(Level::FATAL,   s, enable, out_line);
}

const char* TerminalLogger::level_key(Level lvl) {
	switch (lvl) {
		case Level::INFO:    return "INFO";
		case Level::DEBUG:    return "DEBUG";
		case Level::WARNING: return "WARNING";
		case Level::ERROR:   return "ERROR";
		case Level::FATAL:   return "FATAL";
	}
	return "INFO";
}

const char* TerminalLogger::level_color(Level lvl) {
	switch (lvl) {
		case Level::INFO:    return WHITE;
		case Level::DEBUG:    return BRIGHT_BLU;
		case Level::WARNING: return YELLOW;
		case Level::ERROR:   return RED;
		case Level::FATAL:   return BRIGHT_RED;
	}
	return WHITE;
}

std::string TerminalLogger::format_timestamp_local(std::int64_t epoch_ns) {
	std::time_t seconds = static_cast<std::time_t>(epoch_ns / 1'000'000'000);
	long ns = static_cast<long>(epoch_ns % 1'000'000'000);

	std::tm tm{};
#if defined(_WIN32)
	localtime_s(&tm, &seconds);
#else
	localtime_r(&seconds, &tm);
#endif

	char buf[64];
	std::snprintf(buf, sizeof(buf),
				"%04d-%02d-%02d %02d:%02d:%02d.%09ld",
				tm.tm_year + 1900,
				tm.tm_mon + 1,
				tm.tm_mday,
				tm.tm_hour,
				tm.tm_min,
				tm.tm_sec,
				ns);

	return std::string(buf);
}

void TerminalLogger::log(Level lvl, std::string_view msg, bool enable /* = true */, std::string* out_line /* = nullptr */) {
	using clock = std::chrono::system_clock;
	const auto ns =  std::chrono::duration_cast<std::chrono::nanoseconds>(
						clock::now().time_since_epoch()).count();
	const auto ts = format_timestamp_local(ns);

	if(enable){
		std::ostream& out = std::cout;
		out << level_color(lvl)
			<< ts << " [" << level_key(lvl) << "] "
			<< id_ << ": " << msg
			<< WHITE
			<< '\n';
		out.flush();
	}

	// Optionally producing line for file logging
	if (out_line) {
		out_line->clear();
		out_line->reserve(64 + id_.size() + msg.size());
		out_line->append(ts);
		out_line->append(" [");
		out_line->append(level_key(lvl));
		out_line->append("] ");
		out_line->append(id_);
		out_line->append(": ");
		out_line->append(msg);
	}
}