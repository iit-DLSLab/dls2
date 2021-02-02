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
#ifndef CALLBACK_SUBSCRIBER_TPP_E9ANNCP0
#define CALLBACK_SUBSCRIBER_TPP_E9ANNCP0

#include "dls2/util/messaging/callback_subscriber.hpp"

// TODO temp include
#include <iostream>

namespace dls
{
	template <class PubSub_t>
	CallbackSubscriber<PubSub_t>::CallbackSubscriber
	(
		const std::string &topic,
		callback_t callback_
	) :
		SubscriberBase<PubSub_t>(topic),
		callback(callback_),
		callback_impl(nullptr)
	{ }

	template <class PubSub_t>
	CallbackSubscriber<PubSub_t>::CallbackSubscriber
	(
		const std::string &topic,
		callback_impl_t callback_
	) :
		SubscriberBase<PubSub_t>(topic),
		callback(nullptr),
		callback_impl(callback_)
	{ }

	template <class PubSub_t>
	void CallbackSubscriber<PubSub_t>::onNewDataMessage
	(
		eprosima::fastrtps::Subscriber *sub
	)
	{
		if(callback_impl)
		{
			callback_impl(sub);
		}
		else
		{
			typename PubSub_t::type msg;
			eprosima::fastrtps::SampleInfo_t info;
			if(sub->takeNextData((void*)&msg, &info))
			{
				// if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
				// {
					callback(msg);
				// }
			}
		}
	}
} // end namespace dls

#endif /* end of include guard: CALLBACK_SUBSCRIBER_TPP_E9ANNCP0 */
