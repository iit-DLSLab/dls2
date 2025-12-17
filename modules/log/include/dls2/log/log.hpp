
#ifndef LOG_HPP_AMCC9JXS
#define LOG_HPP_AMCC9JXS

#include "dls2/domains/domains.hpp"
#include "dls2/topics/topics.hpp"
#include "dls2/util/messaging/dds_writer.hpp"
#include "dls2/util/time/duration_utils.hpp"

#include "event_config.hpp"
#include "dls2/util/messaging/dds_reader.hpp"
#include "dls2/util/messaging/dds_writer.hpp"
#include <boost/circular_buffer.hpp>

#include <streambuf>
#include <mutex>
namespace dls
{
	namespace logging
	{
		class LogStreamBuffer : public std::streambuf
		{
		public:
			LogStreamBuffer
			(
				dls::topicType topic_,
				std::size_t buffer_size = 512,
				std::string prefix_ = ""
			);
			~LogStreamBuffer();

			LogStreamBuffer(const LogStreamBuffer&) = delete;
			LogStreamBuffer &operator=(const LogStreamBuffer&) = delete;

		private:
			int_type overflow(int_type ch) override;
			int sync() override;
			bool flush_buffer();
			dls::topicType topic;
			std::shared_ptr<dls::DDSWriter> ddsLogging;

			char *buf;
			const std::string prefix;
		};

		/// Debug log stream
		///
		/// Use this log stream to log debugging data
		class cdbgstream : public std::ostream
		{
		public:
			cdbgstream(const std::string &prefix, std::size_t buffer_size = 512);
			~cdbgstream();
		};

		/// Log log stream
		///
		/// Use this stream to log system events, such as 'controller loaded', 'motion
		/// generator exited' etc.
		class clogstream : public std::ostream
		{
		public:
			clogstream(const std::string &prefix, std::size_t buffer_size = 512);
			~clogstream();
		};

		/// Print log stream
		///
		/// Use this class to log messages that should also be shown to the user
		class warnstream : public std::ostream
		{
		public:
			warnstream(const std::string &prefix, std::size_t buffer_size = 512);
			~warnstream();
		};

		/// Error log stream
		///
		/// Use this log stream to report errors that occurred, but from which the
		/// system can recover. Also log possible future fatal errors for the operator's
		/// attention here.
		class cerrstream : public std::ostream
		{
		public:
			cerrstream(const std::string &prefix, std::size_t buffer_size = 512);
			~cerrstream();
		};

		/// Fatal error log stream
		///
		/// Log fatal errors that occured here. These should be errors from which the
		/// system cannot recover without operator intervention.
		class cfatalstream : public std::ostream
		{
		public:
			cfatalstream(const std::string &prefix, std::size_t buffer_size = 512);
			~cfatalstream();
		};

		class EventNotifier{
		public:
			EventNotifier(const std::string &name);

			virtual ~EventNotifier() = default;

			virtual void notify(const EventID& event_id, const EventSeverity& severity, const std::string &message="");

			std::string get_name() const;

			dls2_interface::msg::EventLog getMsg() const;

			bool hasMatched();

			bool waitForMatch();

		private:
			const std::string name;
			dls2_interface::msg::EventLog msg;
			std::shared_ptr<dls::DDSWriter> dds_writer;
		};

		/**
		 * @brief EventNotifier wrapper implementing event type-level antispamming
		 * and safety check enable flag 
		 */
		class RobustEventNotifier : public EventNotifier{
		public:
			RobustEventNotifier(const std::string &name, const double &spamming_threshold = 250.0);
			
			void setSpammingThreshold(const double &spamming_threshold);
			double getSpammingThreshold();

			virtual void notify(const EventID& event_id, const EventSeverity& severity, const std::string &message="");

		private:
			std::map<EventID, std::chrono::steady_clock::time_point> last_event_pub_time_;
			double spamming_threshold_ms_;
		};

		class EventListener{
		public:
			EventListener(const std::string &name);

			virtual ~EventListener() = default;

			std::string get_name() const;

			int getNumOfMatches() const;

			/// \brief Get the unbounded index of the last value written in the buffer
			///
			/// \return The unbounded index of the last value written in the buffer
			unsigned long long int getUnboundedBufferIdx() const;

			/// \brief Get the maximum index of the buffer
			///
			/// \return The maximum index of the buffer
			unsigned long int getBufferMaxIdx() const;

			boost::circular_buffer<dls2_interface::msg::EventLog> event_buffer;

		private:
			const std::string name;
			std::shared_ptr<dls::DDSReader> dds_reader;

			long long int unbounded_buffer_idx;
			long int buffer_max_idx;
		};
	} // namespace logging
} // namespace dls

#endif /* end of include guard: LOG_HPP_AMCC9JXS */
