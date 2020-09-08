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

// =============================================================================
// Old Includes -- To be removed
// =============================================================================
#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/subscriber/SampleInfo.h>
#include <fastrtps/participant/Participant.h>
#include <mutex>

// =============================================================================
// New Includes
// =============================================================================
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

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
}

// =============================================================================
// New Version
// =============================================================================
/// \cond doxygen_namespace_dls
namespace dls
{
	/// \cond doxygen_namespace_version2
	///
	/// Temporary namespace until the old publishers and subscribers are
	/// refactored into those contained here, then this namespace will be
	/// removed and its contents lifted to the dls namespace
	namespace version2
	{
		template <class PubSub_t>
		class Subscriber
		{
		public:
			typedef std::function<void(typename PubSub_t::type&)> callback_t;
			Subscriber(const std::string &topic, callback_t callback);
			virtual ~Subscriber();

		private:

			eprosima::fastdds::dds::DomainParticipant *participant;
			eprosima::fastdds::dds::Subscriber        *subscriber;
			eprosima::fastdds::dds::DataReader        *reader;
			eprosima::fastdds::dds::Topic             *topic;
			eprosima::fastdds::dds::TypeSupport       type;

			class SubListener : public eprosima::fastdds::dds::DataReaderListener
			{
			public:
				SubListener(callback_t callback);

				void on_subscription_matched
				(
					eprosima::fastdds::dds::DataReader*,
					const eprosima::fastdds::dds::SubscriptionMatchedStatus &info
				) override;
				std::atomic_int sample_count;

				void on_data_available
				(
					eprosima::fastdds::dds::DataReader*
				) override;

				callback_t callback;
			} subscriber_listener;

			typename PubSub_t::type msg;
			PubSub_t rtps_type;
		};
	} /// \endcond namespace version2
} /// \endcond namespace dls

#include "dls2/util/messaging/subscriber_base.tpp"

#endif /* end of include guard: SUBSCRIBER_BASE_HPP_XPACOJJI */
