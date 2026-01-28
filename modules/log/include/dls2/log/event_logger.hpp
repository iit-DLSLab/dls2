
#pragma once

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
			const std::string name_;
			dls2_interface::msg::EventLog msg_;
			std::shared_ptr<dls::DDSWriter> dds_writer_;
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

			std::vector<dls2_interface::msg::EventLog> readEvents();

			boost::circular_buffer<dls2_interface::msg::EventLog> event_buffer_;

		private:
			const std::string name_;
			std::shared_ptr<dls::DDSReader> dds_reader_;

			long long int unbounded_buffer_idx_;
			long int idx_read_ { 0 };
			long int buffer_max_idx_;
		};

	} // namespace logging
} // namespace dls