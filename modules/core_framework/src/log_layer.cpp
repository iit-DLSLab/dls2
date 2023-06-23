/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef LOG_LAYER_CPP_DLJLOFSG
#define LOG_LAYER_CPP_DLJLOFSG

#include "dls2/core_framework/log_layer.hpp"
#include <sys/time.h>
#include <cstdio>

using namespace dls;

LogLayer::LogLayer(std::string ID)
	: Layer(ID)
	, ddsLogLink_(
		"log_layer",
		dls::domains::logging
	)
{
	//debug_log
	ddsLogLink_.addReader(
		"debug_log",
		dls::topics::debug_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": DEBUG: " <<  msg->msg() << std::flush;
			}
		}
	);

	//info_log
	ddsLogLink_.addReader(
		"info_log",
		dls::topics::info_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": INFO: " <<  msg->msg() << std::flush;
			}
		}
	);

	//warn_log
	ddsLogLink_.addReader(
		"warn_log",
		dls::topics::warn_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": WARN: " <<  msg->msg() << std::flush;
			}
		}
	);

	//error_log
	ddsLogLink_.addReader(
		"error_log",
		dls::topics::error_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": ERROR: " <<  msg->msg() << std::flush;
			}
		}
	);

	//fatal_log
	ddsLogLink_.addReader(
		"fatal_log",
		dls::topics::fatal_log_stream,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg *msg = (StringMsg*) tuple;
				std::cout << '\r' << LogLayer::get_current_time() << ": FATAL: " <<  msg->msg() << std::flush;
			}
		}
	);

	// Start recording an MCAP log file
	command_manager.addCommand<>
    (
        "startRecording",
        "Start recording an MCAP log file",
        std::function<bool()>([&]()->bool
        {
			foxserver_.record_mcap_log(true, LogLayer::get_current_time());
			return true;
        }),
        {{}},
        true
    );

	// Stop recording the MCAP log file
	command_manager.addCommand<>
    (
        "stopRecording",
        "Stop recording the MCAP log file",
        std::function<bool()>([&]()->bool
        {
			foxserver_.record_mcap_log(false);
			return true;
        }),
        {{}},
        true
    );
}

AppStatus LogLayer::run()
{
	while(!this->should_quit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return this->getStatus();
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

std::string LogLayer::get_current_time()
{
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[256];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%y-%m-%d_%H.%M.%S", &tstruct);

	return buf;
}
#endif /* end of include guard: LOG_LAYER_CPP_DLJLOFSG */
