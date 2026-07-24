
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

void LogLayer::printEvents(){
	static long int idx_read = 0;
	auto events = event_listener_.readEvents(idx_read);
	for(const auto& event_log : events){
		// print event log
		std::string timestamp = Time::convertTimeToDate(event_log.header().timestamp());
		// std::cout << "\nEvent from component: " << event_log.component_name() << "\n"
		// 			<< "Timestamp:" << timestamp << "\n"
		// 			<< "Sequence ID: " << event_log.header().sequence_id() << "\n"
		// 			<< "Event ID: " << magic_enum::enum_name(static_cast<EventID>(event_log.event_id())) << "\n"
		// 			<< "Severity: " << magic_enum::enum_name(static_cast<EventSeverity>(event_log.severity())) << "\n"
		// 			<< "Message: " << event_log.msg() << std::endl;
	}
}
void LogLayer::monitor(){
	// printEvents();
}
#endif /* end of include guard: LOG_LAYER_CPP_DLJLOFSG */
