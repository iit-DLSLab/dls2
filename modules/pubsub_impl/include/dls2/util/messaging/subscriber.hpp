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
#ifndef SUBSCRIBER_HPP_XPACOJJI
#define SUBSCRIBER_HPP_XPACOJJI

// =============================================================================
// Old Includes -- To be removed
// =============================================================================
#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/subscriber/SampleInfo.h>
#include <fastrtps/participant/Participant.h>
#include <mutex>

#include <functional>

// =============================================================================
// Old Version - To Be Removed
// =============================================================================
namespace dls
{
	template <class PubSub_t>
	class SubscriberBase : public eprosima::fastrtps::SubscriberListener
	{
	public:
		SubscriberBase(const std::string &topic);
		virtual ~SubscriberBase();

	private:
		// std::shared_ptr<eprosima::fastrtps::Participant> pParticipant;
		// std::shared_ptr<eprosima::fastrtps::Subscriber> pSubscriber;
		eprosima::fastrtps::Participant *pParticipant;
		eprosima::fastrtps::Subscriber *pSubscriber;

		static PubSub_t rtps_type;

		// begin critical section
			// static std::mutex ID_mutex;
			// static size_t ID;
		// end critical section
	};
} /// \endcond namespace dls

#include "dls2/util/messaging/subscriber.tpp"

#endif /* end of include guard: SUBSCRIBER_HPP_XPACOJJI */
