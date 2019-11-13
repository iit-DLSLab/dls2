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
#ifndef LOG_LAYER_HPP_IAHZ5BZG
#define LOG_LAYER_HPP_IAHZ5BZG

#include "application_framework/app_layer.hpp"
#include "util/messaging/subscriber_base.hpp"
#include "msg/stringmsgPubSubTypes.h"

#include "topics/debug_log_stream.hpp"
#include "topics/error_log_stream.hpp"
#include "topics/fatal_log_stream.hpp"
#include "topics/info_log_stream.hpp"
#include "topics/warn_log_stream.hpp"
#include "util/debug/debug.hpp"

#include <mutex>
#include <condition_variable>

/// THIS class is in a very hackish copy-paste state at the moment. It will be
/// made better
class LogLayer : public AppLayer
{
public:
	LogLayer();

	Status run() override;
	Status shutdown() override;

private:
	class DebugListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		DebugListener() :
			SubscriberBase<StringMsgPubSubType>(topics::debug_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << msg.msg() << std::endl;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} debug_listener;
	class InfoListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		InfoListener() :
			SubscriberBase<StringMsgPubSubType>(topics::info_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << msg.msg() << std::endl;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} info_listener;
	class WarnListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		WarnListener() :
			SubscriberBase<StringMsgPubSubType>(topics::warn_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << msg.msg() << std::endl;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} warn_listener;
	class ErrorListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		ErrorListener() :
			SubscriberBase<StringMsgPubSubType>(topics::error_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << msg.msg() << std::endl;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} error_listener;
	class FatalListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		FatalListener() :
			SubscriberBase<StringMsgPubSubType>(topics::fatal_log_stream),
			info()
		{ }

		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
		{
			StringMsg msg;
			if(sub->takeNextData((void*)&msg, &info))
			{
				if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				{
					std::cout << msg.msg() << std::endl;
				}
			}
		}
	private:
		eprosima::fastrtps::SampleInfo_t info;
	} fatal_listener;

	// BEGIN critical section
		bool                     should_quit;
		std::condition_variable  should_quit_cv;
		std::mutex               should_quit_mutex;
	// END critical section
};

#endif /* end of include guard: LOG_LAYER_HPP_IAHZ5BZG */
