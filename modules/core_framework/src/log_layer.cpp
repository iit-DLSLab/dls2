
#ifndef LOG_LAYER_CPP_DLJLOFSG
#define LOG_LAYER_CPP_DLJLOFSG

#include "dls2/core_framework/log_layer.hpp"
#include <sys/time.h>
#include <cstdio>

using namespace dls;

LogLayer::LogLayer(std::string ID)
	: Layer(ID)
	, dds_participant_(std::make_shared<dls::DDSParticipant>("log_layer", dls::domains::logging))
{
	//debug_log
	dds_participant_->addReader(
		"debug_log",
		dls::topics::debug_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << get_current_time() << ": DEBUG: " <<  msg->msg() << std::flush;
			}
		}
	);

	//info_log
	dds_participant_->addReader(
		"info_log",
		dls::topics::info_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << get_current_time() << ": INFO: " <<  msg->msg() << std::flush;
			}
		}
	);

	//warn_log
	dds_participant_->addReader(
		"warn_log",
		dls::topics::warn_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << get_current_time() << ": WARN: " <<  msg->msg() << std::flush;
			}
		}
	);

	//error_log
	dds_participant_->addReader(
		"error_log",
		dls::topics::error_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << get_current_time() << ": ERROR: " <<  msg->msg() << std::flush;
			}
		}
	);

	//fatal_log
	dds_participant_->addReader(
		"fatal_log",
		dls::topics::fatal_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << get_current_time() << ": FATAL: " <<  msg->msg() << std::flush;
			}
		}
	);
}

AppStatus LogLayer::run()
{
	while(!this->should_quit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return getStatus();
}

AppStatus LogLayer::stop()
{
	int i = 0;
	while(this->getParticipant()->getParticipants().size() > 1 && i < 10)
	{
		i++;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	this->should_quit = true;
	return getStatus();
}

#endif /* end of include guard: LOG_LAYER_CPP_DLJLOFSG */
