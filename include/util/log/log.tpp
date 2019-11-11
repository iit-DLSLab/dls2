#ifndef LOG_TPP_O9SQ6AB1
#define LOG_TPP_O9SQ6AB1

#include "util/log/log.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>

namespace logging
{
	namespace impl
	{
		template <LogLevel loglevel>
		LogInput<loglevel> &LogInput<loglevel>::operator<<(const std::string &s)
		{
			this->ss << s;
			return *this;
		}

		template <LogLevel loglevel>
		void LogInput<loglevel>::operator<<
		(
			const StreamManip<StreamManipulator::ENDL>&
		)
		{
			LogStream<loglevel>::log(this->ss.str());
		}

		template <LogLevel loglevel>
		LogInput<loglevel> LogStream<loglevel>::operator<<(const std::string &s) const
		{
			logging::impl::LogInput<loglevel> input;

			auto t = std::time(nullptr);
			auto tm = *std::localtime(&t);
			input.ss << std::put_time(&tm, "%d/%m/%Y %H:%M:%S");
			input.ss << ": ";
			input.ss << s;
			return input;
		}

		template <LogLevel loglevel>
		void LogStream<loglevel>::log(const std::string &s)
		{
			std::cout << s << std::endl;
		}

	}
}
#endif /* end of include guard: LOG_TPP_O9SQ6AB1 */
