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
#include "util/log/log.hpp"
#include "util/messaging/publisher_base.hpp"
#include "msg/stringmsgPubSubTypes.h"

#include "topics/debug_log_stream.hpp"
#include "topics/error_log_stream.hpp"
#include "topics/fatal_log_stream.hpp"
#include "topics/info_log_stream.hpp"
#include "topics/warn_log_stream.hpp"

// =============================================================================
// Public Global Objects
// =============================================================================
namespace dls
{
	namespace logging
	{
		LogStreamBuffer bcdbg(dls::topics::debug_log_stream);
		LogStreamBuffer bclog(dls::topics::info_log_stream);
		LogStreamBuffer bcout(dls::topics::warn_log_stream);
		LogStreamBuffer bcerr(dls::topics::error_log_stream);
		LogStreamBuffer bcfatal(dls::topics::fatal_log_stream);

		std::ostream cdbg(&bcdbg);
		std::ostream clog(&bclog);
		std::ostream cout(&bcout);
		std::ostream cerr(&bcerr);
		std::ostream cfatal(&bcfatal);
	}
}

// =============================================================================
// Private Global Objects
// =============================================================================
// std::shared_ptr<dls::PublisherBase<StringMsgPubSubType>> pub = nullptr;
// (dls::topics::debug_log_stream);

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls::logging;

// =============================================================================
// Constructors
// =============================================================================
LogStreamBuffer::LogStreamBuffer(const std::string &topic_, std::size_t buffer_size) :
	topic(topic_),
	pPublisher(nullptr),
	buf(new char[buffer_size])
{
	setp(buf, buf + buffer_size -1);
}

LogStreamBuffer::~LogStreamBuffer()
{
	delete[] buf;
}
// =============================================================================
// Interface Override
// =============================================================================
LogStreamBuffer::int_type LogStreamBuffer::overflow(int_type ch)
{
	if(ch != traits_type::eof())
	{
		*pptr() = ch;
		pbump(1);
		if(flush_buffer()) return ch;
	}

	return traits_type::eof();
}

int LogStreamBuffer::sync()
{
	return flush_buffer()? 0 : -1;
}
bool LogStreamBuffer::flush_buffer()
{
	StringMsg msg;
	msg.msg(std::string(buf, pptr()));

	// Done here, since if it's done statically (for the global cdb, clog, cout,
	// cerr, cfatal classes, then fastrtps complains
	if(pPublisher == nullptr)
	{
		pPublisher = std::make_shared<dls::PublisherBase<StringMsgPubSubType>>(this->topic);
		// pPublisher->publish(msg);
	}
	pPublisher->publish(msg);
	// std::cout << std::string(buf, pptr());
	auto n = pptr() - pbase();
	pbump(-n);
	return true;
}
