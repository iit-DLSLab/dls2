
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
}

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
}

std::string EventNotifier::get_name() const
{
	return name;
}

dls2_interface::msg::EventLog EventNotifier::getMsg() const
{
	return msg;
}

bool EventNotifier::hasMatched()
{
	return dds_writer->hasMatched();
}

bool EventNotifier::waitForMatch()
{
	while(!dds_writer->hasMatched())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return true;
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
}

std::string EventListener::get_name() const
{
	return name;
}

int EventListener::getNumOfMatches() const
{
	return dds_reader->getNumOfMatches();
}

unsigned long long int EventListener::getUnboundedBufferIdx() const
{
	return unbounded_buffer_idx;
}

unsigned long int EventListener::getBufferMaxIdx() const
{
	return event_buffer.capacity()-1;
}
