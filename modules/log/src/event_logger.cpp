
#include "dls2/log/event_logger.hpp"
#include "magic_enum/magic_enum.hpp"

#include <chrono>
#include <thread>
#include <climits>

using namespace dls::logging;

// =============================================================================
// EventNotifier
// =============================================================================
EventNotifier::EventNotifier(const std::string &name)
	: name_(name)
{
	msg_.component_name() = name;
	msg_.header().sequence_id() = 0;

	eprosima::fastdds::dds::DataWriterQos qos(eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);
	qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
	qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
	dds_writer_ = std::make_shared<dls::DDSWriter>(
				name,
				dls::domains::logging,
				dls::topics::log_events,
				qos
	);
}

void EventNotifier::notify(
	const EventID& event_id,
	const EventSeverity& severity,
	const std::string &message
)
{
	msg_.event_id() = static_cast<uint8_t>(event_id);
	msg_.severity() = static_cast<uint8_t>(severity);
	msg_.msg() = message;
	msg_.header().sequence_id()++;
	msg_.header().timestamp() = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	dds_writer_->sendMessage(&msg_);
}

std::string EventNotifier::get_name() const
{
	return name_;
}

dls2_interface::msg::EventLog EventNotifier::getMsg() const
{
	return msg_;
}

bool EventNotifier::hasMatched()
{
	return dds_writer_->hasMatched();
}

bool EventNotifier::waitForMatch()
{
	while(!dds_writer_->hasMatched())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return true;
}

// =============================================================================
// RobustEventNotifier
// =============================================================================
RobustEventNotifier::RobustEventNotifier(const std::string &name, 
										 const double &spamming_threshold)
: EventNotifier(name)
{
	spamming_threshold_ms_ = spamming_threshold;
}

void RobustEventNotifier::notify(const EventID& event_id, 
							const EventSeverity& severity, 
							const std::string &message)
{
	auto now = std::chrono::steady_clock::now();

	const auto last_it = last_event_pub_time_.find(event_id);
	auto publish = (last_it == last_event_pub_time_.end());

	if(!publish){
		const auto spamming_event = toMs(now - last_it->second) < spamming_threshold_ms_;
		publish = !spamming_event;
	}

	if(publish){
		EventNotifier::notify(event_id, severity, message);
		last_event_pub_time_.insert_or_assign(event_id, now);
	}
}

void RobustEventNotifier::setSpammingThreshold(const double &spamming_threshold){
	spamming_threshold_ms_ = spamming_threshold;
}

// =============================================================================
// EventListener
// =============================================================================
EventListener::EventListener(const std::string &name)
	: event_buffer_(1000000), name_(name), unbounded_buffer_idx_(-1) {
	eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
	qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
	qos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;

	dds_reader_ = std::make_shared<dls::DDSReader>(
				name_,
				dls::domains::logging,
				dls::topics::log_events,
				std::function<void(void *)>
				{
					[&](void *tuple)
					{
						dls2_interface::msg::EventLog *msg = (dls2_interface::msg::EventLog*) tuple;
						// std::cout 	<< "###################"
						// 			<< "\ntimestamp: " << msg->header().timestamp()
						// 			<< "\nsequence_id: " << msg->header().sequence_id()
						// 			<< "\nevent: " <<magic_enum::enum_name(static_cast<EventID>(msg->event_id()))
						// 		  	<< "\nseverity: " << magic_enum::enum_name(static_cast<EventSeverity>(msg->severity()))
						// 		  	<< "\ncomponent: " << msg->component_name()
						// 		  	<< "\nmessage: " << msg->msg()
						// 		  	<< "\n###################" << std::endl;
						event_buffer_.push_back(*msg);
						// }
						unbounded_buffer_idx_++;
						if(unbounded_buffer_idx_ > LLONG_MAX)
						{
							throw std::runtime_error("Unbounded buffer index of EventLister exceeded ULLONG_MAX");
						}
					}
				},
				qos);
}

std::string EventListener::get_name() const
{
	return name_;
}

int EventListener::getNumOfMatches() const
{
	return dds_reader_->getNumOfMatches();
}

unsigned long long int EventListener::getUnboundedBufferIdx() const
{
	return unbounded_buffer_idx_;
}

unsigned long int EventListener::getBufferMaxIdx() const
{
	return event_buffer_.capacity()-1;
}
