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
#include "dls2/log/log.hpp"
#include "dls2/util/messaging/publisher.hpp"
#include "dls2/msg/stringmsgPubSubTypes.h"

#include "dls2/topics/topics.hpp"

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls::logging;

// =============================================================================
// LogStreamBuffer
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
LogStreamBuffer::LogStreamBuffer
(
	const std::string &topic_,
	std::size_t buffer_size,
	const std::string &prefix_
) :
	topic(topic_),
	// pPublisher(
	// 	std::make_shared<version2::Publisher<StringMsgPubSubType>>(
	// 		dls::domains::logging,
	// 		prefix_,
	// 		topic_
	// 	)
	// ),
	buf(new char[buffer_size]),
	prefix(prefix_ + ": ")
{
	setp(buf, buf + buffer_size -1);
}

LogStreamBuffer::~LogStreamBuffer()
{
	delete[] buf;
}
// -----------------------------------------------------------------------------
// Interface Override
// -----------------------------------------------------------------------------
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
	std::shared_ptr<StringMsg> msg(new StringMsg());
	msg->msg(this->prefix + std::string(buf, pptr()));

	// Done here, since if it's done statically (for the global cdb, clog, cout,
	// cerr, cfatal classes, then fastrtps complains
	// if(pPublisher == nullptr)
	// {
	// 	pPublisher = std::make_shared<dls::PublisherBase<StringMsgPubSubType>>(this->topic);
	// }
	pPublisher->publish(msg.get());
	// std::cout << std::string(buf, pptr());
	auto n = pptr() - pbase();
	pbump(-n);
	return true;
}

// =============================================================================
// Streams
// =============================================================================
// -----------------------------------------------------------------------------
// Debug Stream
// -----------------------------------------------------------------------------
cdbgstream::cdbgstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::debug_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

cdbgstream::~cdbgstream()
{
	delete rdbuf();
}
// -----------------------------------------------------------------------------
// Log Stream
// -----------------------------------------------------------------------------
clogstream::clogstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::info_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

clogstream::~clogstream()
{
	delete rdbuf();
}
// -----------------------------------------------------------------------------
// Cout Stream
// -----------------------------------------------------------------------------
coutstream::coutstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::warn_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

coutstream::~coutstream()
{
	delete rdbuf();
}
// -----------------------------------------------------------------------------
// Error Stream
// -----------------------------------------------------------------------------
cerrstream::cerrstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::error_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

cerrstream::~cerrstream()
{
	delete rdbuf();
}
// -----------------------------------------------------------------------------
// Fatal Error Stream
// -----------------------------------------------------------------------------
cfatalstream::cfatalstream(const std::string &prefix, std::size_t buffer_size) :
	std::ostream
	(
		new LogStreamBuffer
		(
			dls::topics::fatal_log_stream,
			buffer_size,
			prefix
		)
	)
{ }

cfatalstream::~cfatalstream()
{
	delete rdbuf();
}
