
#ifndef DDSPARTICIPANT_HPP
#define DDSPARTICIPANT_HPP

#include "dls2/topics/topics.hpp"
#include "dls2/domains/domains.hpp"
#include "dls2/util/messaging/dds_listeners.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastrtps/types/DynamicDataHelper.hpp>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

#include <map>
#include <set>
#include <string>
#include "yaml-cpp/yaml.h"

/// \cond doxygen_namespace_dls
namespace dls
{
	class DDSParticipant : public eprosima::fastdds::dds::DomainParticipantListener 
	{

	public:
		DDSParticipant(
			std::string	partName_,
			dls::domainType domain_,
			eprosima::fastrtps::rtps::DiscoveryProtocol_t part_type = eprosima::fastrtps::rtps::DiscoveryProtocol_t::CLIENT,
			bool tupelookup_server = true
		);

		virtual ~DDSParticipant();

		/// Retrieves a list of all participants in the domain of the participant
		///
		std::vector<std::string> getParticipants();
		std::vector<std::string> getDiscoveredTopics();

		std::multimap<std::string, eprosima::fastrtps::rtps::GUID_t> getDiscoveredParticipantsInfo();

		

		eprosima::fastdds::dds::DataWriter* getWriter(std::string);
		eprosima::fastdds::dds::DataReader* getReader(std::string);

		std::shared_ptr<dls::DDSSubListener> getSubListener(const std::string&);
		std::shared_ptr<dls::DDSPubListener> getPubListener(const std::string&);

		eprosima::fastdds::dds::DataWriter* addWriter(
			std::string    writerName,
			dls::topicType topicData,
			eprosima::fastdds::dds::DataWriterQos qos = eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT
		);

		eprosima::fastdds::dds::DataReader* addReader(
			std::string                 readerName,
			dls::topicType				topicData,
			std::function<void(void *)>	callback,
			eprosima::fastdds::dds::DataReaderQos qos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT
		);

		eprosima::fastdds::dds::DataReader *addReader(
			std::string readerName,
			std::string topicName,
			std::function<void(void *)> callback,
			eprosima::fastdds::dds::DataReaderQos qos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT
		);

		bool deleteReader(const std::string& reader_name);
		bool deleteWriter(const std::string& writer_name);

		bool sendMessage(std::string writerName, void *msg);

		void setTopicListener(dls::DDSPartListener *listener_);

		bool is_type_registered_in_participant_(const std::string& type_name);

		eprosima::fastdds::dds::Topic * getTopicFromString(const std::string& topic_name);

		std::string getTypeNameFromTopic(const std::string& topic_name);

		bool topicFound(const std::string& topic_name);
		
		std::string getName() const;		
		

		
		
	private:
		std::string server_ip;
		int server_port;
		std::string server_guid_prefix;

		std::string participant_name;

		eprosima::fastdds::dds::DomainParticipant  					*participant;
		std::map<std::string, eprosima::fastdds::dds::Topic *>  	topics;	
		std::map<std::string, eprosima::fastdds::dds::DataReader *> readers;
		std::map<std::string, eprosima::fastdds::dds::DataWriter *> writers;
		std::map<std::string, std::shared_ptr<dls::DDSSubListener>>	subListeners;
		std::map<std::string, std::shared_ptr<dls::DDSPubListener>>	pubListeners;

		eprosima::fastdds::dds::Publisher  *publisher;
        eprosima::fastdds::dds::Subscriber *subscriber;
		
		dls::DDSPartListener *topicListener;

		std::unordered_map<std::string, std::string> discovery_database;

		

		YAML::Node config;

		eprosima::fastdds::dds::Topic* addTopic(dls::topicType topicData_);
		eprosima::fastdds::dds::Topic* addTopic(std::string topicName);

		std::multimap<std::string, eprosima::fastrtps::rtps::GUID_t> discovered_participants_info;

		/*!
		* @brief Custom Callback on_participant_discovery
		* @param[in] participant domain participant discovering a new domain participant
		* @param[in] info information about the discovered domain participant
		*/
		void on_participant_discovery(
			eprosima::fastdds::dds::DomainParticipant* participant,
			eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info) override;

		void on_publisher_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info) override;

		void on_subscriber_discovery(
                eprosima::fastdds::dds::DomainParticipant* participant,
                eprosima::fastrtps::rtps::ReaderDiscoveryInfo&& info) override;
        

		void on_topic_discovery_(const std::string& topic_name, const std::string& type_name);

		void on_type_information_received(
			eprosima::fastdds::dds::DomainParticipant*,
			const eprosima::fastrtps::string_255 topic_name,
			const eprosima::fastrtps::string_255 type_name,
			const eprosima::fastrtps::types::TypeInformation& type_information) override;

		void on_type_discovery(
				eprosima::fastdds::dds::DomainParticipant* participant,
				const eprosima::fastrtps::rtps::SampleIdentity& request_sample_id,
				const eprosima::fastrtps::string_255& topic,
				const eprosima::fastrtps::types::TypeIdentifier* identifier,
				const eprosima::fastrtps::types::TypeObject* object,
				eprosima::fastrtps::types::DynamicType_ptr dyn_type) override;

		bool is_type_registered_in_xml_(const std::string& type_name);
		bool is_type_registered_in_factory_(const std::string& type_name);
		eprosima::fastrtps::types::DynamicType_ptr get_type_registered_(const std::string& type_name);
	};
	
} // namespace dls
/// \endcond
#endif // DDSPARTICIPANT_HPP