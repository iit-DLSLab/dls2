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
#ifndef LOG_HPP_AMCC9JXS
#define LOG_HPP_AMCC9JXS

#include <streambuf>
#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/msg/stringmsgPubSubTypes.h"
#include "dls2/domains/domains.hpp"

namespace dls
{
namespace logging
{
class LogStreamBuffer : public std::streambuf
{
public:
	LogStreamBuffer
	(
		const std::string &topic,
		std::size_t buffer_size = 512,
		const std::string &prefix = ""
	);
	~LogStreamBuffer();

	LogStreamBuffer(const LogStreamBuffer&) = delete;
	LogStreamBuffer &operator=(const LogStreamBuffer&) = delete;

private:
	int_type overflow(int_type ch) override;
	int sync() override;
	bool flush_buffer();
	std::string topic;
	std::shared_ptr<version2::Publisher<StringMsgPubSubType>> pPublisher;

	char *buf;
	const std::string prefix;
};

/// Debug log stream
///
/// Use this log stream to log debugging data
class cdbgstream : public std::ostream
{
public:
	cdbgstream(const std::string &prefix, std::size_t buffer_size = 512);
	~cdbgstream();
};

/// Log log stream
///
/// Use this stream to log system events, such as 'controller loaded', 'gait
/// generator exited' etc.
class clogstream : public std::ostream
{
public:
	clogstream(const std::string &prefix, std::size_t buffer_size = 512);
	~clogstream();
};

/// Print log stream
///
/// Use this class to log messages that should also be shown to the user
class coutstream : public std::ostream
{
public:
	coutstream(const std::string &prefix, std::size_t buffer_size = 512);
	~coutstream();
};

/// Error log stream
///
/// Use this log stream to report errors that occurred, but from which the
/// system can recover. Also log possible future fatal errors for the operator's
/// attention here.
class cerrstream : public std::ostream
{
public:
	cerrstream(const std::string &prefix, std::size_t buffer_size = 512);
	~cerrstream();
};

/// Fatal error log stream
///
/// Log fatal errors that occured here. These should be errors from which the
/// system cannot recover without operator intervention.
class cfatalstream : public std::ostream
{
public:
	cfatalstream(const std::string &prefix, std::size_t buffer_size = 512);
	~cfatalstream();
};

} // namespace logging
} // namespace dls

#endif /* end of include guard: LOG_HPP_AMCC9JXS */
