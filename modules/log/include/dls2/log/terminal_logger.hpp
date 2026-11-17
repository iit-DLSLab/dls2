#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <cstdint>

namespace dls
{
	namespace logging
	{
        static constexpr const char* WHITE = "\033[0m";
        static constexpr const char* GREEN = "\033[32m";
        static constexpr const char* YELLOW = "\033[33m";
        static constexpr const char* RED = "\033[31m";
        static constexpr const char* BRIGHT_RED = "\033[1;31m";
        static constexpr const char* BRIGHT_BLU = "\033[1;36m";
        
		class TerminalLogger {
			public:
				explicit TerminalLogger(const std::string& ID);

				void info(const std::string& s, bool enable = true, std::string* out_line = nullptr);
				void debug(const std::string& s, bool enable = true, std::string* out_line = nullptr);
				void warning(const std::string& s, bool enable = true, std::string* out_line = nullptr);
				void error(const std::string& s, bool enable = true, std::string* out_line = nullptr);
				void fatal(const std::string& s, bool enable = true, std::string* out_line = nullptr);

			private:
				enum class Level { 
                    INFO, 
                    DEBUG, 
                    WARNING, 
                    ERROR, 
                    FATAL 
                };

				static const char* level_key(Level lvl);
				static const char* level_color(Level lvl);
				static std::string format_timestamp_local(std::int64_t epoch_ns);

				void log(Level lvl, std::string_view msg, bool enable = true, std::string* out_line = nullptr);

				std::string id_;
			};

        } // namespace logging
} // namespace dls