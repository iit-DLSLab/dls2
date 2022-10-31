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
#ifndef DDSPARTICIPANT_HPP
#define DDSPARTICIPANT_HPP

#include "dls2/topics/topics.hpp"
#include "dls2/domains/domains.hpp"
#include "dls2/util/messaging/dds_listeners.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

#include <map>
#include <set>
#include <string>

/// \cond doxygen_namespace_dls
namespace dls
{
	class DDSParticipant : public eprosima::fastdds::dds::DomainParticipantListener 
	{

	public:
		DDSParticipant(
			std::string 	partName_,
			dls::domainType domain_
		);

		~DDSParticipant();

		/// Retrieves a list of all participants in the domain of the participant
		///
		std::vector<std::string> getParticipants();

		eprosima::fastdds::dds::DataWriter *addWriter(
			std::string    writerName,
			dls::topicType topicData
		);

		eprosima::fastdds::dds::DataReader *addReader(
			std::string                 readerName,
			dls::topicType				topicData,
			std::function<void(void *)>	callback
		);

		bool sendMessage(std::string writerName, void *msg);

		void setTopicListener(dls::DDSPartListener *listener_);

	private:
		eprosima::fastdds::dds::DomainParticipant  					*participant;
		std::map<std::string, eprosima::fastdds::dds::Topic *>  	topics;	
		std::set<std::string>										types;
		std::map<std::string, eprosima::fastdds::dds::DataReader *> readers;
		std::map<std::string, eprosima::fastdds::dds::DataWriter *> writers;
		std::vector<dls::DDSSubListener *>							subListeners;

		eprosima::fastdds::dds::Publisher  *publisher;
        eprosima::fastdds::dds::Subscriber *subscriber;
		
		eprosima::fastdds::dds::Topic* addTopic(dls::topicType topicData_);

		void on_publisher_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info) override;

		void on_topic_discovery_(const std::string& topic_name, const std::string& type_name);

		dls::DDSPartListener *topicListener;
		std::unordered_map<std::string, std::string> discovery_database;
	};
	
} // namespace dls
/// \endcond
#endif // DDSPARTICIPANT_HPP