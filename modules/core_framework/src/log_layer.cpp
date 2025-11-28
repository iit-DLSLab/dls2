
#ifndef LOG_LAYER_CPP_DLJLOFSG
#define LOG_LAYER_CPP_DLJLOFSG

#include "magic_enum/magic_enum.hpp"
#include "dls2/core_framework/log_layer.hpp"
#include <sys/time.h>
#include <cstdio>
#include <iomanip>

using namespace dls;

LogLayer::LogLayer(std::string ID)
	: Layer(ID, 50)
	, dds_participant_(std::make_shared<dls::DDSParticipant>("log_layer", dls::domains::logging))
	, event_listener_(ID)
{
	// Define QoS for the data reader
	eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
	qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
	qos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;

	//debug_log
	dds_participant_->addReader(
		"debug_log",
		dls::topics::debug_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				dls2_interface::msg::String *msg = (dls2_interface::msg::String*) tuple;
				std::cout << '\r' << get_current_time() << ": DEBUG: " <<  msg->msg() << std::flush;
			}
		},
		qos
	);

	//info_log
	dds_participant_->addReader(
		"info_log",
		dls::topics::info_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				dls2_interface::msg::String *msg = (dls2_interface::msg::String*) tuple;
				std::cout << '\r' << get_current_time() << ": INFO: " <<  msg->msg() << std::flush;
			}
		},
		qos
	);

	//warn_log
	dds_participant_->addReader(
		"warn_log",
		dls::topics::warn_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				dls2_interface::msg::String *msg = (dls2_interface::msg::String*) tuple;
				std::cout << '\r' << get_current_time() << ": WARN: " <<  msg->msg() << std::flush;
			}
		},
		qos
	);

	//error_log
	dds_participant_->addReader(
		"error_log",
		dls::topics::error_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				dls2_interface::msg::String *msg = (dls2_interface::msg::String*) tuple;
				std::cout << '\r' << get_current_time() << ": ERROR: " <<  msg->msg() << std::flush;
			}
		},
		qos
	);

	//fatal_log
	dds_participant_->addReader(
		"fatal_log",
		dls::topics::fatal_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				dls2_interface::msg::String *msg = (dls2_interface::msg::String*) tuple;
				std::cout << '\r' << get_current_time() << ": FATAL: " <<  msg->msg() << std::flush;
			}
		},
		qos
	);
}

void LogLayer::close()
{
	// bool close = false;
	// while(!close)
	// {
	// 	auto participants = ddspart_layer->getParticipants();
	// 	if(participants.size()<=2) // if it is two, only the server for layers and the LogLayer itself are left
	// 		close = true;
	// 	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	// }
}

std::string LogLayer::convertTimeToDate(long int timestamp){
	time_t time_val = static_cast<time_t>(timestamp/1000000000); // timestamp in nanoseconds
	std::tm* utc_tm = std::localtime(&time_val);
    std::ostringstream oss;
    oss << std::put_time(utc_tm, "%y-%m-%d_%H.%M.%S");
    return oss.str();
} // end convertTimeToString

void LogLayer::readEvents(){
	static long int idx_read = 0;
	static long int buffer_max_idx = event_listener_.getBufferMaxIdx();
	long long int idx_buffer = event_listener_.getUnboundedBufferIdx();
	if(idx_buffer>=idx_read){
		// mapping unbounded indexes in bounded indexes
		long int delta = idx_buffer - idx_read;
		if(delta> buffer_max_idx){
			idx_read = 0;
			idx_buffer = buffer_max_idx;
		}
		else if (idx_buffer >= buffer_max_idx)
		{
			idx_read = buffer_max_idx - delta;
			idx_buffer = idx_read + delta;
		}
		// read values
		for(long int i=idx_read; i<=idx_buffer; ++i)
		{
			dls2_interface::msg::EventLog event_log = event_listener_.event_buffer[i];
			// print event log
			std::string timestamp = convertTimeToDate(event_log.header().timestamp());
			std::cout << "\nEvent from component: " << event_log.component_name() << "\n"
					  << "Timestamp:" << timestamp << "\n"
					  << "Sequence ID: " << event_log.header().sequence_id() << "\n"
					  << "Event ID: " << magic_enum::enum_name(static_cast<EventID>(event_log.event_id())) << "\n"
					  << "Severity: " << magic_enum::enum_name(static_cast<EventSeverity>(event_log.severity())) << "\n"
					  << "Message: " << event_log.msg() << std::endl;
		}
		// update read index
		idx_read = idx_buffer+1;
	}
}
void LogLayer::monitor(){
	readEvents();
}
#endif /* end of include guard: LOG_LAYER_CPP_DLJLOFSG */
