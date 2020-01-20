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
#ifndef PUBLISHER_BASE_TPP_I5UWXWN8
#define PUBLISHER_BASE_TPP_I5UWXWN8

#include "dls2/util/messaging/publisher_base.hpp"

#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/TopicDataType.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>

#include "dls2/util/debug/debug.hpp"

namespace dls
{
	// template <class PubSub_t>
	// PubSub_t PublisherBase<PubSub_t>::rtps_type;

	template <class PubSub_t>
	PublisherBase<PubSub_t>::PublisherBase(const std::string &topic) :
		pParticipant(nullptr),
		pPublisher(nullptr),
		rtps_type(),
		temp_topic(topic)
	{
		// Create participant
		eprosima::fastrtps::ParticipantAttributes participant_attr;
		participant_attr.rtps.builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;
		participant_attr.rtps.builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol = true;
		participant_attr.rtps.builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
		participant_attr.rtps.builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
		participant_attr.rtps.builtin.domainId = 0;
		participant_attr.rtps.builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
		participant_attr.rtps.setName("Participant_pub");

		// participant_attr.rtps.setName("Participant_publisher");

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
		// TODO Check for null pointer above

		// register
		eprosima::fastrtps::Domain::registerType
		(
			pParticipant.get(), static_cast<eprosima::fastrtps::TopicDataType*>(&rtps_type)
		);

		// Create publisher
		eprosima::fastrtps::PublisherAttributes pub_attr;
		pub_attr.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
		pub_attr.topic.topicDataType = rtps_type.getName();
		pub_attr.topic.topicName = topic;
		pub_attr.topic.historyQos.kind = eprosima::fastrtps::KEEP_LAST_HISTORY_QOS;
		pub_attr.topic.historyQos.depth = 30;
		pub_attr.topic.resourceLimitsQos.max_samples = 50;
		pub_attr.topic.resourceLimitsQos.allocated_samples = 20;
		pub_attr.times.heartbeatPeriod.seconds = 2;
		pub_attr.times.heartbeatPeriod.nanosec = 200*1000*1000;
		pub_attr.qos.m_reliability.kind = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;


		// pub_attr.qos.m_reliability.kind = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;
		// pub_attr.topic.historyQos.kind = eprosima::fastrtps::KEEP_ALL_HISTORY_QOS;
		// pub_attr.qos.m_durability.kind = eprosima::fastrtps::VOLATILE_DURABILITY_QOS;
		// TODO("Change the name here");
		// pub_attr.qos.m_liveliness.announcement_period = 0.5;
		// pub_attr.qos.m_liveliness.lease_duration = 1;
		// pub_attr.qos.m_liveliness.kind = eprosima::fastrtps::AUTOMATIC_LIVELINESS_QOS;

		// TODO not cleaning the publisher because it's generating a library error
		pPublisher.reset
		(
			eprosima::fastrtps::Domain::createPublisher
			(
				pParticipant.get(),
				pub_attr,
				this
			),
			[](eprosima::fastrtps::Publisher*){}
		);
		// TODO Check nullptr above
	}

	template<class PubSub_t>
	void PublisherBase<PubSub_t>::publish(typename PubSub_t::type &msg) const
	{
		pPublisher->write(&msg);
	}

	template<class PubSub_t>
	PublisherBase<PubSub_t>::~PublisherBase()
	{ }
} // end namespace dls

#endif /* end of include guard: PUBLISHER_BASE_TPP_I5UWXWN8 */
