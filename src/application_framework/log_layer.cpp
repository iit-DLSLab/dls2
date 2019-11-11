#ifndef LOG_LAYER_CPP_DLJLOFSG
#define LOG_LAYER_CPP_DLJLOFSG

#include "util/log/log.hpp"

template <enum LogLevel>
log::impl::cout<LogLevel> operator<< (log::impl::cout<log::impl::LogLevel> log&, const std::string &s)
{
	log.ss << s;
	return log;
}


friend template <enum LogLevel>
void operator<< (log::impl::cout<log::impl::LogLevel> log&, const log::endl&)
{
	std::cout << log.ss.str();
}

#endif /* end of include guard: LOG_LAYER_CPP_DLJLOFSG */
