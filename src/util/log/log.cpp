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

// =============================================================================
// Public Global Objects
// =============================================================================
namespace dls
{
	namespace logging
	{
		LogStreamBuffer b0;
		LogStreamBuffer b1;
		LogStreamBuffer b2;
		LogStreamBuffer b3;
		LogStreamBuffer b4;

		std::ostream cdbg(&b0);
		std::ostream clog(&b1);
		std::ostream cout(&b2);
		std::ostream cerr(&b3);
		std::ostream cfatal(&b4);
	}
}

// =============================================================================
// Private Global Objects
// =============================================================================
std::shared_ptr<dls::PublisherBase<StringMsgPubSubType>> pub = nullptr;
// (dls::topics::debug_log_stream);

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls::logging;

// =============================================================================
// Constructors
// =============================================================================
LogStreamBuffer::LogStreamBuffer(std::size_t buffer_size) :
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
	if(pub == nullptr)
	{
		pub = std::make_shared<dls::PublisherBase<StringMsgPubSubType>>(dls::topics::debug_log_stream);
	}
	pub->publish(msg);
	// std::cout << std::string(buf, pptr());
	auto n = pptr() - pbase();
	pbump(-n);
	return true;
}
