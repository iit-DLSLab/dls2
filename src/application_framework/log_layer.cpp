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

#include "dls2/application_framework/log_layer.phpp"
#include <sys/time.h>
#include <cstdio>

// The current release of Eigen defines in the file `Eigen_Colamd.h` the macro:
// # define ALIVE (0)
// This macro seems to be refered to in only that file. Being a very generic
// name, it clashes with the enumeration in: `eprosima::fastrtps::rtps::ALIVE`
// As of this writing, it seems that this has been fixed in Eigen's github.
// However, it has not been officially released yet. At the time of this writing
// (21 January 2020), the latest stable release of Eigen is Eigen 3.3.7,
// released on 11 December 2018. Until such time as Eigen's fix makes it into a
// new release, the following lines solves the issue.
//
// For this message, and other similar issues, refer to the wiki page `Software
// Issues`
//
//                                                                     - Hendrik
#ifdef ALIVE
#undef ALIVE
#endif

using namespace dls;
LogLayer::LogLayer() :
	AppLayer("log_layer"),
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
