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
#ifndef SUBSCRIBER_TPP_YC8QGBV2
#define SUBSCRIBER_TPP_YC8QGBV2

// =============================================================================
// Old Includes -- To be removed
// =============================================================================
#include "dls2/util/messaging/subscriber.hpp"

#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/participant/Participant.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/TopicDataType.h>
#include <fastrtps/Domain.h>

#include <stdexcept>
#include <sstream>

// =============================================================================
// Old Version - To be Removed
// =============================================================================
namespace dls
{
	template <class PubSub_t>
	PubSub_t SubscriberBase<PubSub_t>::rtps_type;

	// template<class PubSub_t>
	// std::mutex SubscriberBase<PubSub_t>::ID_mutex;

	// template<class PubSub_t>
	// size_t SubscriberBase<PubSub_t>::ID = 0;

	template <class PubSub_t>
	SubscriberBase<PubSub_t>::SubscriberBase(const std::string &topic) :
		pParticipant(nullptr),
		pSubscriber(nullptr)
	{
		eprosima::fastrtps::ParticipantAttributes participant_attr;
		participant_attr.rtps.builtin.discovery_config.discoveryProtocol                                      = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;
		participant_attr.rtps.builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol                   = true;
		participant_attr.rtps.builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
		participant_attr.rtps.builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
		participant_attr.rtps.builtin.discovery_config.leaseDuration                                          = eprosima::fastrtps::c_TimeInfinite;
		participant_attr.rtps.setName("Participant_sub");

		this->pParticipant = eprosima::fastrtps::Domain::createParticipant(participant_attr);
		if(!this->pParticipant)
		{
			throw std::runtime_error
			(
				"Could not build subscriber participant on topic '" + topic + "'"
			);
		}

		eprosima::fastrtps::Domain::registerType
		(
			pParticipant,
			static_cast<eprosima::fastrtps::TopicDataType*>
			(
				&rtps_type
				// &SubscriberBase<PubSub_t>::rtps_type
			)
		);

		eprosima::fastrtps::SubscriberAttributes sub_attr;
		sub_attr.topic.topicKind                           = eprosima::fastrtps::rtps::NO_KEY;
		sub_attr.topic.topicDataType                       = rtps_type.getName();
		sub_attr.topic.topicName                           = topic;
		sub_attr.topic.historyQos.kind                     = eprosima::fastrtps::KEEP_LAST_HISTORY_QOS;
		sub_attr.topic.historyQos.depth                    = 30;
		sub_attr.topic.resourceLimitsQos.max_samples       = 50;
		sub_attr.topic.resourceLimitsQos.allocated_samples = 50;
		sub_attr.qos.m_reliability.kind                    = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;
		sub_attr.qos.m_durability.kind                     = eprosima::fastrtps::TRANSIENT_LOCAL_DURABILITY_QOS;

		this->pSubscriber = eprosima::fastrtps::Domain::createSubscriber
		(
			this->pParticipant,
			sub_attr,
			static_cast<eprosima::fastrtps::SubscriberListener*>(this)
		);

		if(!this->pSubscriber)
		{
			throw std::runtime_error
			(
				"Could not build subscriber on topic '" + topic + "'"
			);
		}
	}

	template <class PubSub_t>
	SubscriberBase<PubSub_t>::~SubscriberBase()
	{
		eprosima::fastrtps::Domain::removeParticipant(this->pParticipant);
	}
} // end namespace dls

#endif /* end of include guard: SUBSCRIBER_TPP_YC8QGBV2 */
