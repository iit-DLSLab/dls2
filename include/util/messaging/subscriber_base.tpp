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
#ifndef SUBSCRIBER_BASE_TPP_YC8QGBV2
#define SUBSCRIBER_BASE_TPP_YC8QGBV2

#include "util/messaging/subscriber_base.hpp"

#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/TopicDataType.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <sstream>

namespace dls
{
	template <class PubSub_t>
	PubSub_t SubscriberBase<PubSub_t>::rtps_type;

	template<class PubSub_t>
	std::mutex SubscriberBase<PubSub_t>::ID_mutex;

	template<class PubSub_t>
	size_t SubscriberBase<PubSub_t>::ID = 0;

	template <class PubSub_t>
	SubscriberBase<PubSub_t>::SubscriberBase(const std::string &topic) :
		pParticipant(nullptr),
		pSubscriber(nullptr)
	{
		eprosima::fastrtps::ParticipantAttributes participant_attr;
		participant_attr.rtps.builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;
		participant_attr.rtps.builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol = true;
		participant_attr.rtps.builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
		participant_attr.rtps.builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
		participant_attr.rtps.builtin.domainId = 0;
		participant_attr.rtps.builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;

		std::stringstream ss;
		ss << "Participant_sub";
		{
			std::lock_guard<std::mutex> lock(SubscriberBase<PubSub_t>::ID_mutex);
			ss << SubscriberBase<PubSub_t>::ID;
			++SubscriberBase<PubSub_t>::ID;
		}
		participant_attr.rtps.setName(ss.str().c_str());

		// auto custom_transport = std::make_shared<eprosima::fastrtps::rtps::UDPv4TransportDescriptor>();
		// custom_transport->interfaceWhiteList.emplace_back("127.0.0.1");
		// participant_attr.rtps.useBuiltinTransports = false;
		// participant_attr.rtps.userTransports.push_back(custom_transport);

		pParticipant.reset
		(
			eprosima::fastrtps::Domain::createParticipant(participant_attr),
			[](eprosima::fastrtps::Participant*){}
			// [](eprosima::fastrtps::Participant *p)
			// {
			// 	eprosima::fastrtps::Domain::removeParticipant(p);
			// }
		);
		eprosima::fastrtps::Domain::registerType
		(
			pParticipant.get(),
			static_cast<eprosima::fastrtps::TopicDataType*>(&rtps_type)
		);

		eprosima::fastrtps::SubscriberAttributes sub_attr;
		sub_attr.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
		sub_attr.topic.topicDataType = rtps_type.getName();
		sub_attr.topic.topicName = topic;
		sub_attr.topic.historyQos.kind = eprosima::fastrtps::KEEP_LAST_HISTORY_QOS;
		sub_attr.topic.historyQos.depth = 30;
		sub_attr.topic.resourceLimitsQos.max_samples = 50;
		sub_attr.topic.resourceLimitsQos.allocated_samples = 50;
		sub_attr.qos.m_reliability.kind = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;
		sub_attr.qos.m_durability.kind = eprosima::fastrtps::TRANSIENT_LOCAL_DURABILITY_QOS;


		// sub_attr.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
		// sub_attr.topic.topicDataType = rtps_type.getName();
		// sub_attr.qos.m_reliability.kind = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;
		// sub_attr.topic.historyQos.kind = eprosima::fastrtps::KEEP_ALL_HISTORY_QOS;
		// sub_attr.qos.m_durability.kind = eprosima::fastrtps::VOLATILE_DURABILITY_QOS;
		// sub_attr.qos.m_liveliness.lease_duration = 1;
		// sub_attr.qos.m_liveliness.kind = eprosima::fastrtps::AUTOMATIC_LIVELINESS_QOS;
		// sub_attr.topic.topicName = topic;

		TODO("figure out how to remove this subscriber")
		pSubscriber.reset
		(
			eprosima::fastrtps::Domain::createSubscriber
			(
				pParticipant.get(),
				sub_attr,
				static_cast<eprosima::fastrtps::SubscriberListener*>(this)
			),
			[](eprosima::fastrtps::Subscriber*){}
		);
	}
} // end namespace dls

#endif /* end of include guard: SUBSCRIBER_BASE_TPP_YC8QGBV2 */
