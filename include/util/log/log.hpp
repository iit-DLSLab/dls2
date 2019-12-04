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
#ifndef LOG_HPP_AMCC9JXS
#define LOG_HPP_AMCC9JXS

#include <streambuf>
#include "util/messaging/publisher_base.hpp"
#include "msg/stringmsgPubSubTypes.h"

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
	std::shared_ptr<PublisherBase<StringMsgPubSubType>> pPublisher;

	char *buf;
	const std::string prefix;
};

class cdbgstream : public std::ostream
{
public:
	cdbgstream(const std::string &prefix, std::size_t buffer_size = 512);
	~cdbgstream();
};

class clogstream : public std::ostream
{
public:
	clogstream(const std::string &prefix, std::size_t buffer_size = 512);
	~clogstream();
};

class coutstream : public std::ostream
{
public:
	coutstream(const std::string &prefix, std::size_t buffer_size = 512);
	~coutstream();
};

class cerrstream : public std::ostream
{
public:
	cerrstream(const std::string &prefix, std::size_t buffer_size = 512);
	~cerrstream();
};

class cfatalstream : public std::ostream
{
public:
	cfatalstream(const std::string &prefix, std::size_t buffer_size = 512);
	~cfatalstream();
};

} // namespace logging
} // namespace dls

#endif /* end of include guard: LOG_HPP_AMCC9JXS */
