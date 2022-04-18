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
#ifndef PUBLISHER_TPP_I5UWXWN8
#define PUBLISHER_TPP_I5UWXWN8

// =============================================================================
// Old Includes -- to be removed
// =============================================================================
#include "dls2/util/messaging/publisher.hpp"

#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/participant/Participant.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/TopicDataType.h>
#include <fastrtps/Domain.h>

#include <stdexcept>

// =============================================================================
// Old Version -- to be removed
// =============================================================================
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
		participant_attr.rtps.builtin.discovery_config.discoveryProtocol                                      = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;
		participant_attr.rtps.builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol                   = true;
		participant_attr.rtps.builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
		participant_attr.rtps.builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
		// participant_attr.rtps.builtin.domainId                                                                = 0;
		participant_attr.rtps.builtin.discovery_config.leaseDuration                                          = eprosima::fastrtps::c_TimeInfinite;
		participant_attr.rtps.setName((const char *) topic.c_str());

		this->pParticipant = eprosima::fastrtps::Domain::createParticipant(participant_attr);

		if(!this->pParticipant)
		{
			throw std::runtime_error
			(
				"Could not build publisher participant on topic '" + topic + "'"
			);
		}

		// register
		eprosima::fastrtps::Domain::registerType
		(
			pParticipant, static_cast<eprosima::fastrtps::TopicDataType*>(&rtps_type)
		);

		// Create publisher
		eprosima::fastrtps::PublisherAttributes pub_attr;
		pub_attr.topic.topicKind                           = eprosima::fastrtps::rtps::NO_KEY;
		pub_attr.topic.topicDataType                       = rtps_type.getName();
		pub_attr.topic.topicName                           = topic;
		pub_attr.topic.historyQos.kind                     = eprosima::fastrtps::KEEP_LAST_HISTORY_QOS;
		pub_attr.topic.historyQos.depth                    = 30;
		pub_attr.topic.resourceLimitsQos.max_samples       = 50;
		pub_attr.topic.resourceLimitsQos.allocated_samples = 20;
		pub_attr.times.heartbeatPeriod.seconds             = 2;
		pub_attr.times.heartbeatPeriod.nanosec             = 200*1000*1000;
		pub_attr.qos.m_reliability.kind                    = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;

		this->pPublisher = eprosima::fastrtps::Domain::createPublisher
		(
			this->pParticipant,
			pub_attr,
			static_cast<eprosima::fastrtps::PublisherListener*>(this)
		);

		if(!this->pPublisher)
		{
			throw std::runtime_error
			(
				"Could not build publisher on topic '" + topic + "'"
			);
		}
	}

	template<class PubSub_t>
	void PublisherBase<PubSub_t>::publish(typename PubSub_t::type &msg) const
	{
		pPublisher->write(&msg);
	}

	template<class PubSub_t>
	PublisherBase<PubSub_t>::~PublisherBase()
	{
		eprosima::fastrtps::Domain::removeParticipant(this->pParticipant);
	}

	template<class PubSub_t>
	eprosima::fastrtps::rtps::GUID_t PublisherBase<PubSub_t>::getGuid() const
	{
		return this->pPublisher->getGuid();
	}
} // end namespace dls
#endif /* end of include guard: PUBLISHER_TPP_I5UWXWN8 */
