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
#ifndef CALLBACK_SUBSCRIBER_HPP_LBFF8X5Q
#define CALLBACK_SUBSCRIBER_HPP_LBFF8X5Q
// =============================================================================
// Includes
// =============================================================================
#include "dls2/util/messaging/subscriber.hpp"

namespace dls
{
	/// A simple subscriber class that calls a given callback when a message is
	/// received
	///
	template <class PubSub_t>
	class CallbackSubscriber : public SubscriberBase<PubSub_t>
	{
		// defined a friend to access the advanced constructor
		template <typename U, typename T>
		friend class Service;

	public:
		// typedef void(*callback_t)(typename PubSub_t::type&);
		typedef std::function<void(typename PubSub_t::type&)> callback_t;
		CallbackSubscriber
		(
			const std::string &topic,
			callback_t callback
		);

	private:
		// implementation callback that should only be used by the framework,
		// not third party code
		//
		// this allows some mechanisms to directly access the underlying
		// fastrtps interface. This is not exposed, since fastrtps should be
		// completely hidden from client code
		typedef std::function <void(eprosima::fastrtps::Subscriber *sub)>
			callback_impl_t;

		// advanced constructor that should only be used from framework code
		// that needs access to the underlying fastrtps interface
		CallbackSubscriber
		(
			const std::string &topic,
			callback_impl_t callback
		);

		void onNewDataMessage
		(
			eprosima::fastrtps::Subscriber *sub
		) override;

		callback_t      callback;
		callback_impl_t callback_impl;
	};
} // end namespace dls

#include "dls2/util/messaging/callback_subscriber.tpp"

#endif /* end of include guard: CALLBACK_SUBSCRIBER_HPP_LBFF8X5Q */
