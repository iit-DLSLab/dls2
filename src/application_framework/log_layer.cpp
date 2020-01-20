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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef LOG_LAYER_CPP_DLJLOFSG
#define LOG_LAYER_CPP_DLJLOFSG

#include "dls2/application_framework/log_layer.hpp"
#include <sys/time.h>
#include <cstdio>

using namespace dls;
LogLayer::LogLayer() :
	debug_listener(),
	info_listener(),
	warn_listener(),
	error_listener(),
	fatal_listener(),
	raw_listener(),
	should_quit(false),
	should_quit_cv(),
	should_quit_mutex()
{ }

LogLayer::Status LogLayer::run()
{
	// TODO start subscribers

	std::unique_lock<std::mutex> lock(this->should_quit_mutex);
	this->should_quit_cv.wait
	(
		lock,
		[&]{return this->should_quit;}
	);

	// TODO stop subscribers

	return this->getStatus();
}

LogLayer::Status LogLayer::shutdown()
{
	{
		std::lock_guard<std::mutex> lock(this->should_quit_mutex);
		this->should_quit = true;
	}
	this->should_quit_cv.notify_one();

	return this->getStatus();
}

std::string LogLayer::get_current_time()
{
	char buf [256];
	struct timeval tv;
	struct timezone tz;
	struct tm *tm;
	gettimeofday(&tv, &tz);
	tm=localtime(&tv.tv_sec);
	sprintf(buf, "%02d:%02d:%02d:%03d:%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec/1000), (int)(tv.tv_usec%1000));
	return buf;
}
#endif /* end of include guard: LOG_LAYER_CPP_DLJLOFSG */
