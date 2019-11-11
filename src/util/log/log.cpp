#include "util/log/log.hpp"
namespace logging
{
	impl::LogStream<impl::LogLevel::DEBUG>            cdbg;
	impl::LogStream<impl::LogLevel::INFO>             clog;
	impl::LogStream<impl::LogLevel::WARN>             cout;
	impl::LogStream<impl::LogLevel::ERROR>            cerr;
	impl::LogStream<impl::LogLevel::FATAL>            cfatal;
	impl::StreamManip<impl::StreamManipulator::ENDL>  endl;

	namespace impl
	{
		template<> const std::string LogStream<LogLevel::DEBUG>::topic(topics::debug_log_stream);
		template<> std::shared_ptr<PublisherBase<StringMsgPubSubType>> LogStream<LogLevel::DEBUG>::pPublisher = nullptr;

		template<> const std::string LogStream<LogLevel::INFO>::topic(topics::info_log_stream);
		template<> std::shared_ptr<PublisherBase<StringMsgPubSubType>> LogStream<LogLevel::INFO>::pPublisher = nullptr;

		template<> const std::string LogStream<LogLevel::WARN>::topic(topics::warn_log_stream);
		template<> std::shared_ptr<PublisherBase<StringMsgPubSubType>> LogStream<LogLevel::WARN>::pPublisher = nullptr;

		template<> const std::string LogStream<LogLevel::ERROR>::topic(topics::error_log_stream);
		template<> std::shared_ptr<PublisherBase<StringMsgPubSubType>> LogStream<LogLevel::ERROR>::pPublisher = nullptr;

		template<> const std::string LogStream<LogLevel::FATAL>::topic(topics::fatal_log_stream);
		template<> std::shared_ptr<PublisherBase<StringMsgPubSubType>> LogStream<LogLevel::FATAL>::pPublisher = nullptr;
	}
}
