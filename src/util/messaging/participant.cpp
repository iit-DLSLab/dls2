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

/// \cond doxygen_namespace_dls
namespace dls
{
	/// \cond doxygen_namespace_impl
	namespace impl
	{
		eprosima::fastdds::dds::DomainParticipant *fastdds_participant =
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
			eprosima::fastdds::dds::DomainParticipantQos participant_qos;
			participant_qos.wire_protocol()
			    .builtin.discovery_config.discoveryProtocol =
			    eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;

			participant_qos
			    .wire_protocol()
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
				topic_name,
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
	} // namespace impl
	  /// \endcond
} // namespace dls
/// \endcond
