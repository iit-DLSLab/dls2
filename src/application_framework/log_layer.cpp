#ifndef LOG_LAYER_CPP_DLJLOFSG
#define LOG_LAYER_CPP_DLJLOFSG

#include "application_framework/log_layer.hpp"

LogLayer::LogLayer() :
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
#endif /* end of include guard: LOG_LAYER_CPP_DLJLOFSG */
