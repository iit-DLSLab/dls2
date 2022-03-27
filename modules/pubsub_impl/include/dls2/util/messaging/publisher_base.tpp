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
#ifndef PUBLISHER_BASE_TPP_I5UWXWN8
#define PUBLISHER_BASE_TPP_I5UWXWN8

// =============================================================================
// Old Includes -- to be removed
// =============================================================================
#include "dls2/util/messaging/publisher_base.hpp"

#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/participant/Participant.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/TopicDataType.h>
#include <fastrtps/Domain.h>

#include <stdexcept>


// =============================================================================
// New Includes
// =============================================================================
#include "dls2/util/messaging/participant.hpp"
#include "dls2/util/messaging/publisher_base.hpp"

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
		Publisher<PubSub_t>::Publisher(
			const unsigned int &domain_,
			const std::string &part_,
			const std::string &topic_
		) :
			participant(nullptr),
			publisher(nullptr),
			topic(nullptr),
			writer(nullptr),
			type(new PubSub_t())
		{
			eprosima::fastdds::dds::DomainParticipantQos participantQos;
			participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;
			participantQos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = eprosima::fastrtps::Duration_t(1, 2);
			// participantQos.wire_protocol().builtin.discovery_config.initial_announcements.count = 2000;
			// participantQos.wire_protocol().builtin.discovery_config.initial_announcements.period = eprosima::fastrtps::Duration_t(0, 100000000u);
			participantQos.name(part_);

			this->participant = eprosima::fastdds::dds::DomainParticipantFactory::
				get_instance()->create_participant(domain_, participantQos);

			if(this->participant == nullptr){
				throw std::runtime_error("Error: could not create participant");
			}
			this->type.register_type(this->participant);
			
			this->topic = this->participant->create_topic(
				topic_, 
				rtps_type.getName(), 
				eprosima::fastdds::dds::TOPIC_QOS_DEFAULT
			);
		
			if(this->topic == nullptr){
				throw std::runtime_error(
					"Error: could not create publisher topic"
				);
			}

			this->publisher = this->participant->create_publisher(
				eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT,
				nullptr
			);

			if(this->publisher == nullptr){
				throw std::runtime_error(
					"Error: could not create publisher"
				);
			}

			this->writer = this->publisher->create_datawriter(
				this->topic,
				eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT,
				&this->publisher_listener
			);

			if(this->writer == nullptr)	{
				throw std::runtime_error(
					"Error: could not create publisher writer"
				);
			}
		}

		template<class PubSub_t>
		Publisher<PubSub_t>::~Publisher(){
			if(this->writer != nullptr){
				this->publisher->delete_datawriter(this->writer);
			}
			
			if(this->topic != nullptr){
			 	this->participant->delete_topic(this->topic);
			}			
			
			if(this->publisher != nullptr){
				this->participant->delete_publisher(this->publisher);
			}
			
			eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->
				delete_participant(this->participant);
		}

		template<class PubSub_t>
		void Publisher<PubSub_t>::publish(typename PubSub_t::type &msg) const{
			if(publisher_listener.matched_count > 0){
				this->writer->write(&msg);
			}
		}

		template<class PubSub_t>
		auto Publisher<PubSub_t>::getGuid() const -> eprosima::fastrtps::rtps::GUID_t
		{
			std::vector<eprosima::fastdds::dds::DataWriter*> writers;
			this->publisher->get_datawriters(writers);
			assert(!writers.empty());
			return writers[0]->guid();
		}

		// =====================================================================
		// Helper Listener Class
		// =====================================================================
		template <class PubSub_t>
		Publisher<PubSub_t>::PublisherListener::PublisherListener()
		{ }

		template <class PubSub_t>
		void Publisher<PubSub_t>::PublisherListener::on_publication_matched
		(
			eprosima::fastdds::dds::DataWriter*,
			const eprosima::fastdds::dds::PublicationMatchedStatus &info
		)
		{
			if(info.current_count_change == 1){
				// publisher matched
				this->matched_count = info.total_count;
			}
			else if(info.current_count_change == -1){
				// publisher unmatched
				this->matched_count = info.total_count;
			}
			else{
				// invalid
			}
		}
	} /// \endcond namespace version2
} /// \endcond namespace dls
#endif /* end of include guard: PUBLISHER_BASE_TPP_I5UWXWN8 */
