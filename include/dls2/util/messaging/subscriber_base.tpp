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

// =============================================================================
// Old Includes -- To be removed
// =============================================================================
#include "dls2/util/messaging/subscriber_base.hpp"

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
// New Includes
// =============================================================================
#include "dls2/util/messaging/subscriber_base.hpp"

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
		// participant_attr.rtps.builtin.domainId                                                                = 0;
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
		Subscriber<PubSub_t>::Subscriber
		(
			const std::string &topic,
			callback_t callback
		) :
			participant(nullptr),
			subscriber(nullptr),
			reader(nullptr),
			topic(nullptr),
			type(new PubSub_t()),
			subscriber_listener(callback)
		{
			eprosima::fastdds::dds::DomainParticipantQos participantQos;
			participantQos.wire_protocol().builtin.discovery_config.initial_announcements.count = 20;
			participantQos.wire_protocol().builtin.discovery_config.initial_announcements.period = eprosima::fastrtps::Duration_t(0, 100000000u);

			participantQos.name("Participant_subscriber");
			this->participant = eprosima::fastdds::dds::DomainParticipantFactory::
				get_instance()->create_participant(0, participantQos);

			this->type.register_type(this->participant);

			if(this->participant == nullptr)
			{
				throw std::runtime_error
				(
					"Error: could not create subscriber participant"
				);
			}

			this->topic = this->participant->create_topic
			(
				topic,
				this->rtps_type.getName(),
				eprosima::fastdds::dds::TOPIC_QOS_DEFAULT
			);

			if(this->topic == nullptr)
			{
				throw std::runtime_error
				(
					"Error: could not create subscriber topic"
				);
			}

			this->subscriber = this->participant->create_subscriber
			(
				eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT,
				nullptr
			);

			if(this->subscriber == nullptr)
			{
				throw std::runtime_error
				(
					"Error: could not create subscriber"
				);
			}

			this->reader = this->subscriber->create_datareader
			(
				this->topic,
				eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT,
				&this->subscriber_listener
			);

			if(this->reader == nullptr)
			{
				throw std::runtime_error
				(
					"Error: could not create subscriber reader"
				);
			}
		}

		template <class PubSub_t>
		Subscriber<PubSub_t>::~Subscriber()
		{
			if(this->reader != nullptr)
			{
				this->subscriber->delete_datareader(this->reader);
			}
			if(this->topic != nullptr)
			{
				this->participant->delete_topic(this->topic);
			}
			if(this->subscriber != nullptr)
			{
				this->participant->delete_subscriber(this->subscriber);
			}
			eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->
				delete_participant(this->participant);
		}

		// =====================================================================
		// Helper Listener Class
		// =====================================================================
		template <class PubSub_t>
		Subscriber<PubSub_t>::SubListener::SubListener(callback_t callback_) :
			sample_count(0),
			callback(callback_),
			msg()
		{ }

		template <class PubSub_t>
		void Subscriber<PubSub_t>::SubListener::on_subscription_matched
		(
			eprosima::fastdds::dds::DataReader*,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus &info
		)
		{
			if(info.current_count_change == 1)
			{
				// subscriber matched
			}
			else if(info.current_count_change == -1)
			{
				// subscriber unmatched
			}
			else
			{
				// invalid
			}
		}

		template <class PubSub_t>
		void Subscriber<PubSub_t>::SubListener::on_data_available
		(
			eprosima::fastdds::dds::DataReader *reader
		)
		{
			eprosima::fastdds::dds::SampleInfo info;
			if
			(
				reader->take_next_sample
				(
					&this->msg,
					&info
				)
				== /*eprosima::fastdds::dds::*/ReturnCode_t::RETCODE_OK
			)
			{
				if(info.instance_state == eprosima::fastdds::dds::ALIVE)
				{
					this->sample_count++;
					this->callback(this->msg);
				}
			}
		}
	} /// \endcond namespace version2
} /// \endcond namespace dls
#endif /* end of include guard: SUBSCRIBER_BASE_TPP_YC8QGBV2 */
