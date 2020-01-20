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
#ifndef SUBSCRIBER_BASE_HPP_XPACOJJI
#define SUBSCRIBER_BASE_HPP_XPACOJJI

#include <memory>
#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/subscriber/SampleInfo.h>
#include <fastrtps/participant/Participant.h>
#include <mutex>

namespace dls
{
	template <class PubSub_t>
	class SubscriberBase : public eprosima::fastrtps::SubscriberListener
	{
	public:
		SubscriberBase(const std::string &topic);
		virtual ~SubscriberBase() = default;

	private:
		std::shared_ptr<eprosima::fastrtps::Participant> pParticipant;
		std::shared_ptr<eprosima::fastrtps::Subscriber> pSubscriber;

		static PubSub_t rtps_type;

		// begin critical section
			static std::mutex ID_mutex;
			static size_t ID;
		// end critical section
	};
}

#include "dls2/util/messaging/subscriber_base.tpp"

#endif /* end of include guard: SUBSCRIBER_BASE_HPP_XPACOJJI */
