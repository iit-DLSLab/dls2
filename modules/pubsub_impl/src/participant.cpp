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
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/participant/Participant.h>

#include "dls2/util/messaging/participant.hpp"

// Legacy
#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/Domain.h>

/// \cond doxygen_namespace_dls
namespace dls
{
	/// \cond doxygen_namespace_impl
	namespace impl
	{
		eprosima::fastdds::dds::DomainParticipant *fastdds_participant =
		    nullptr;

		eprosima::fastrtps::Participant *legacy_fastrtps_participant =
		    nullptr;

		static std::mutex registered_topics_mutex;
		static std::map
		<
			std::string,
			eprosima::fastdds::dds::Topic*
		> registered_topics;

		void initFastdds()
		{
			std::cout << "start init fastdds" << std::endl;
			// ======= Create the participant for the new API of fastdds =======
			eprosima::fastdds::dds::DomainParticipantQos participant_qos;
			participant_qos.wire_protocol()
			    .builtin.discovery_config.discoveryProtocol =
			    eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;

			participant_qos.wire_protocol()
			    .builtin.discovery_config.leaseDuration_announcementperiod =
			    eprosima::fastrtps::Duration_t(1, 2);

			participant_qos.name("Participant_publisher");

			dls::impl::fastdds_participant =
			    eprosima::fastdds::dds::DomainParticipantFactory::get_instance()
			        ->create_participant(0, participant_qos);

			if(dls::impl::fastdds_participant == nullptr)
			{
				throw std::runtime_error(
				    "Error: could not create publisher participant");
			}
/*
			// ====== Create the participant for the old API of fastrtps =======
			// This should eventually be removed when all the old pubs and subs
			// are taken out
			{
				eprosima::fastrtps::ParticipantAttributes participant_attr;
				participant_attr.rtps.builtin.discovery_config
				    .discoveryProtocol =
				    eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;

				participant_attr.rtps.builtin.discovery_config
				    .use_SIMPLE_EndpointDiscoveryProtocol = true;

				participant_attr.rtps.builtin.discovery_config.m_simpleEDP
				    .use_PublicationReaderANDSubscriptionWriter = true;

				participant_attr.rtps.builtin.discovery_config.m_simpleEDP
				    .use_PublicationWriterANDSubscriptionReader = true;

				// participant_attr.rtps.builtin.domainId = 0;
				participant_attr.rtps.builtin.discovery_config.leaseDuration =
				    eprosima::fastrtps::c_TimeInfinite;

				participant_attr.rtps.setName("Participant_pub");

				legacy_fastrtps_participant =
				    eprosima::fastrtps::Domain::createParticipant(
				        participant_attr);

				if(legacy_fastrtps_participant == nullptr)
				{
					throw std::runtime_error(
					    "Could not build legacy participant");
				}
				std::cout << "Built legacy fastrtps participant" << std::endl;
			}
			*/
			std::cout << "initialised fastdds" << std::endl;
		}

		auto getFastddsParticipant() -> eprosima::fastdds::dds::DomainParticipant *
		{
			return fastdds_participant;
		}
		void closeFastdds()
		{
			eprosima::fastdds::dds::DomainParticipantFactory::get_instance()
			    ->delete_participant(dls::impl::fastdds_participant);
		}

		// registers the topic, else returns an already existing topic. This is
		// because fastdds crashes if you register the same topic name twice
		auto registerFastddsTopic(const std::string &topic_name,
		                          const std::string &rtps_type_name)
		    -> eprosima::fastdds::dds::Topic *
		{
			std::lock_guard<std::mutex> lock(registered_topics_mutex);
			auto it = registered_topics.find(topic_name);

			// if the topic is already registred, return it
			if(it != registered_topics.end())
			{
				return it->second;
			}

			// else, register the topic, save it and return it
			auto *dds_topic = dls::impl::getFastddsParticipant()->create_topic
			(
				std::string("rt/")+topic_name,
				rtps_type_name,
				eprosima::fastdds::dds::TOPIC_QOS_DEFAULT
			);
			if(dds_topic == nullptr)
			{
				throw std::runtime_error
				(
					"Error: could not create publisher topic"
				);
			}
			registered_topics[topic_name] = dds_topic;
			return dds_topic;
		}
		/// \cond doxygen_namespace_legacy
		namespace legacy
		{
			auto getFastrtpsLegacyParticipant()
			    -> eprosima::fastrtps::Participant *
			{
				return legacy_fastrtps_participant;
			}
		} /// \endcond namespace legacy
	} // namespace impl
	  /// \endcond
} // namespace dls
/// \endcond
