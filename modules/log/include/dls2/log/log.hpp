
#ifndef LOG_HPP_AMCC9JXS
#define LOG_HPP_AMCC9JXS

#include "dls2/domains/domains.hpp"
#include "dls2/topics/topics.hpp"
#include "dls2/util/messaging/dds_writer.hpp"
#include "event_config.hpp"
#include "dls2/util/messaging/dds_reader.hpp"
#include "dls2/util/messaging/dds_writer.hpp"
#include <dls2/util/time/time.hpp>

#include <boost/circular_buffer.hpp>

#include <streambuf>
#include <mutex>
#include <iostream>
#include <string>
#include <cstddef>
#include <ctime>
#include <cstdio>

namespace dls
{
	namespace logging
	{
		constexpr const char* RED    = "\033[31m";
		constexpr const char* YELLOW = "\033[33m";
		constexpr const char* RESET  = "\033[0m";

		class ColorStreambuf : public std::streambuf {

			public:
				ColorStreambuf(std::ostream& out,
								std::string prefix,
								const char* color_code,
								std::size_t /*buffer_size*/ = 512);
				~ColorStreambuf() override;

			protected:
				int overflow(int ch) override; // Called when a character is inserted
				int sync() override; // Called on std::flush / std::endl and some other cases

			private:
				void flush_line_if_needed(bool flush_even_if_empty);

				std::ostream& out_;
				std::string prefix_;
				const char* color_;
				std::string buffer_;
			};

		/// Debug log stream
		class cdbgstream : public std::ostream 
		{
		public:
			cdbgstream(const std::string& prefix, std::size_t buffer_size = 512);
			~cdbgstream() override;

			void print(const std::string& s);

		private:
			ColorStreambuf buf_;
		};

		/// Log info stream
		class clogstream : public std::ostream 
		{
		public:
			clogstream(const std::string& prefix, std::size_t buffer_size = 512);
			~clogstream() override;

			void print(const std::string& s);

		private:
			ColorStreambuf buf_;
		};

		/// Print log stream
		class warnstream : public std::ostream 
		{
		public:
			warnstream(const std::string& prefix, std::size_t buffer_size = 512);
			~warnstream() override;

			void print(const std::string& s);

		private:
			ColorStreambuf buf_;
		};
		
		/// Error log stream
		class cerrstream : public std::ostream 
		{
		public:
			cerrstream(const std::string& prefix, std::size_t buffer_size = 512);
			~cerrstream() override;

			void print(const std::string& s);

		private:
			ColorStreambuf buf_;
		};

		/// Fatal error log stream
		class cfatalstream : public std::ostream 
		{
		public:
			cfatalstream(const std::string& prefix, std::size_t buffer_size = 512);
			~cfatalstream() override;

			void print(const std::string& s);

		private:
			ColorStreambuf buf_;
		};

		class EventNotifier
		{
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
