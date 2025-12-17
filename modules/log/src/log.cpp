
#include "dls2/log/log.hpp"
#include "magic_enum/magic_enum.hpp"

#include <chrono>
#include <thread>
#include <climits>

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls::logging;

// =============================================================================
// LogStreamBuffer
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
LogStreamBuffer::LogStreamBuffer
(
	dls::topicType topic_,
	std::size_t buffer_size,
	std::string prefix_
)
	: topic(topic_)
	, buf(new char[buffer_size])
	, prefix(prefix_ + ": ")
{
	eprosima::fastdds::dds::DataWriterQos qos(eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);
	qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
	qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
	ddsLogging = std::make_shared<dls::DDSWriter>(
				prefix_,
				dls::domains::logging,
				topic_,
				qos
			);
	setp(buf, buf + buffer_size -1);
}

LogStreamBuffer::~LogStreamBuffer()
{
	delete[] buf;
}
// -----------------------------------------------------------------------------
// Interface Override
// -----------------------------------------------------------------------------
LogStreamBuffer::int_type LogStreamBuffer::overflow(int_type ch)
{
	if(ch != traits_type::eof())
	{
		*pptr() = ch;
		pbump(1);
		if(flush_buffer()) return ch;
	}

	return traits_type::eof();
}

int LogStreamBuffer::sync()
{
	return flush_buffer()? 0 : -1;
}
bool LogStreamBuffer::flush_buffer()
{
	std::shared_ptr<dls2_interface::msg::String> msg(new dls2_interface::msg::String());
	msg->msg(this->prefix + std::string(buf, pptr()));

	this->ddsLogging->sendMessage(msg.get());

	// std::cout << std::string(buf, pptr());
	auto n = pptr() - pbase();
	pbump(-n);
	return true;
}

// =============================================================================
// Streams
// =============================================================================
// -----------------------------------------------------------------------------
// Debug Stream
// -----------------------------------------------------------------------------
cdbgstream::cdbgstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::debug_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

cdbgstream::~cdbgstream()
{
	delete rdbuf();
}
// -----------------------------------------------------------------------------
// Log Stream
// -----------------------------------------------------------------------------
clogstream::clogstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::info_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

clogstream::~clogstream()
{
	delete rdbuf();
}
// -----------------------------------------------------------------------------
// Cout Stream
// -----------------------------------------------------------------------------
warnstream::warnstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::warn_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

warnstream::~warnstream()
{
	delete rdbuf();
}
// -----------------------------------------------------------------------------
// Error Stream
// -----------------------------------------------------------------------------
cerrstream::cerrstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::error_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

cerrstream::~cerrstream()
{
	delete rdbuf();
}
// -----------------------------------------------------------------------------
// Fatal Error Stream
// -----------------------------------------------------------------------------
cfatalstream::cfatalstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::fatal_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

cfatalstream::~cfatalstream()
{
	delete rdbuf();
}

// =============================================================================
// EventNotifier
// =============================================================================
EventNotifier::EventNotifier(const std::string &name)
	: name(name)
{
	msg.component_name() = name;
	msg.header().sequence_id() = 0;

	eprosima::fastdds::dds::DataWriterQos qos(eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);
	qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
	qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
	dds_writer = std::make_shared<dls::DDSWriter>(
				name,
				dls::domains::logging,
				dls::topics::log_events,
				qos
	);
} // end constructor

void EventNotifier::notify(
	const EventID& event_id,
	const EventSeverity& severity,
	const std::string &message
)
{
	msg.event_id() = static_cast<uint8_t>(event_id);
	msg.severity() = static_cast<uint8_t>(severity);
	msg.msg() = message;
	msg.header().sequence_id()++;
	msg.header().timestamp() = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	dds_writer->sendMessage(&msg);
} // end notify

std::string EventNotifier::get_name() const
{
	return name;
} // end get_name

dls2_interface::msg::EventLog EventNotifier::getMsg() const
{
	return msg;
} // end getMsg

bool EventNotifier::hasMatched()
{
	return dds_writer->hasMatched();
} // end hasMatched

bool EventNotifier::waitForMatch()
{
	while(!dds_writer->hasMatched())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return true;
} // end waitForMatch


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
	: event_buffer(1000000), name(name), unbounded_buffer_idx(-1) {
	eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
	qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
	qos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;

	dds_reader = std::make_shared<dls::DDSReader>(
				name,
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
						event_buffer.push_back(*msg);
						// }
						unbounded_buffer_idx++;
						if(unbounded_buffer_idx>LLONG_MAX)
						{
							throw std::runtime_error("Unbounded buffer index of EventLister exceeded ULLONG_MAX");
						}
					}
				},
				qos);
} // end constructor

std::string EventListener::get_name() const
{
	return name;
} // end get_name

int EventListener::getNumOfMatches() const
{
	return dds_reader->getNumOfMatches();
} // end getNumOfMatches

unsigned long long int EventListener::getUnboundedBufferIdx() const
{
	return unbounded_buffer_idx;
} // end getUnboundedBufferIdx

unsigned long int EventListener::getBufferMaxIdx() const
{
	return event_buffer.capacity()-1;
} // end getBufferMaxIdx
