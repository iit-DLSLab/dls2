#ifndef LOG_HPP_AMCC9JXS
#define LOG_HPP_AMCC9JXS

#include <sstream>

namespace logging
{
	// implementation namespace
	namespace impl
	{
		enum class LogLevel
		{
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL
		};

		enum class StreamManipulator
		{
			ENDL
		};

		template <StreamManipulator manip>
		class StreamManip{};

		template <LogLevel loglevel>
		class LogStream;

		template <LogLevel loglevel>
		class LogInput
		{
			friend class LogStream<loglevel>;
		public:
			LogInput<loglevel> &operator<< (const std::string& s)
			{
				this->ss << s;
				return *this;
			}

			void operator<<(const StreamManip<StreamManipulator::ENDL>&)
			{
				LogStream<loglevel>::log(this->ss.str());
			}

		private:
			LogInput() = default;
			std::stringstream ss;
		};

		template <LogLevel loglevel>
		class LogStream
		{
			friend class LogInput<loglevel>;
		public:
			LogInput<loglevel> operator<< (const std::string&s) const
			{
				logging::impl::LogInput<loglevel> input;
				input.ss << s;
				return input;
			}
		private:
			static void log(const std::string &s)
			{
				std::cout << s << std::endl;
			}
		};

	} // namespace impl


	// static members for usage
	impl::LogStream<impl::LogLevel::DEBUG>  cdbg;
	impl::LogStream<impl::LogLevel::INFO>   clog;
	impl::LogStream<impl::LogLevel::WARN>   cout;
	impl::LogStream<impl::LogLevel::ERROR>  cerr;
	impl::LogStream<impl::LogLevel::FATAL>  cfatal;

	impl::StreamManip<impl::StreamManipulator::ENDL> endl;

} // namespace logging

#include "util/log/log.tpp"

#endif /* end of include guard: LOG_HPP_AMCC9JXS */
