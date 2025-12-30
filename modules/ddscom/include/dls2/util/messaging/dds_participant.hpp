
#ifndef DDSPARTICIPANT_HPP
#define DDSPARTICIPANT_HPP

#include "dls2/topics/topics.hpp"
#include "dls2/domains/domains.hpp"
#include "dls2/util/messaging/dds_listeners.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

#include <fastdds/dds/rpc/Replier.hpp>
#include <fastdds/dds/rpc/Requester.hpp>

#include <map>
#include <set>
#include <string>
#include "yaml-cpp/yaml.h"

namespace dds_rpc = eprosima::fastdds::dds::rpc;

/// \cond doxygen_namespace_dls
namespace dls
{
	static constexpr uint32_t MAX_SEQUENCE_ID = 1000;

  class RpcService
  {
  public:
    RpcService(const std::string& name, void* data);

    void sendReply(void* data);
    void sendRequest(void* data);

    std::string _name;
    dds_rpc::Service* _service;
    dds_rpc::Requester* _requester;
    dds_rpc::Replier* _replier;
    dds_rpc::RequestInfo _info;
    void* _data;
  };

	class DDSParticipant : public eprosima::fastdds::dds::DomainParticipantListener
	{

	public:
		DDSParticipant(
			std::string	partName_,
			dls::domainType domain_,
			eprosima::fastdds::rtps::DiscoveryProtocol part_type = eprosima::fastdds::rtps::DiscoveryProtocol::CLIENT
		);

		virtual ~DDSParticipant();

		/// Retrieves a list of all participants in the domain of the participant
		///
		std::vector<std::string> getParticipants();
		std::vector<std::string> getDiscoveredTopics();

		std::multimap<std::string, eprosima::fastdds::rtps::GUID_t> getDiscoveredParticipantsInfo();



		eprosima::fastdds::dds::DataWriter* getWriter(std::string);
		eprosima::fastdds::dds::DataReader* getReader(std::string);

		std::shared_ptr<dls::DDSSubListener> getSubListener(const std::string&);
		std::shared_ptr<dls::DDSPubListener> getPubListener(const std::string&);

		eprosima::fastdds::dds::DataWriter* addWriter(
			std::string    writerName,
			dls::topicType topicData,
			eprosima::fastdds::dds::DataWriterQos qos = eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT,
			bool add_ros2_namespace = true
		);

		eprosima::fastdds::dds::DataReader* addReader(
			std::string                 readerName,
			dls::topicType				topicData,
			std::function<void(void *)>	callback,
			eprosima::fastdds::dds::DataReaderQos qos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT,
			bool add_ros2_namespace = true
		);

		// eprosima::fastdds::dds::DataReader *addReader(
		// 	std::string readerName,
		// 	std::string topicName,
		// 	std::function<void(void *)> callback,
		// 	eprosima::fastdds::dds::DataReaderQos qos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT
		// );

		bool deleteReader(const std::string& reader_name);
		bool deleteWriter(const std::string& writer_name);

		bool sendMessage(std::string writerName, void *msg);

		void setTopicListener(dls::DDSPartListener *listener_);

		bool is_type_registered_in_participant_(const std::string& type_name);

		eprosima::fastdds::dds::Topic * getTopicFromString(const std::string& topic_name);

		std::string getTypeNameFromTopic(const std::string& topic_name);

		bool topicFound(const std::string& topic_name);

		std::string getName() const;

		std::unordered_map<std::string, eprosima::fastdds::dds::TypeSupport> get_discovery_database();

		std::unordered_map<std::string, eprosima::fastdds::dds::DynamicType::_ref_type> get_discovery_database_dyn_types();

		static YAML::Node getServersConfig();

    std::shared_ptr<RpcService> createReplier(const std::string &serviceName,
                                              const dls::topicType &requestTopic,
                                              const dls::topicType &replyTopic,
                                              void* data);

    std::shared_ptr<RpcService> createRequester(const std::string &serviceName,
                                                const dls::topicType &requestTopic,
                                                const dls::topicType &replyTopic,
                                                void* data);

    void deleteService(const std::string &serviceName);

	private:
		std::string server_ip;
		int server_port;

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

		std::unordered_map<std::string, eprosima::fastdds::dds::TypeSupport> discovery_database;
		std::unordered_map<std::string, eprosima::fastdds::dds::DynamicType::_ref_type> discovery_database_dyn_types;

		YAML::Node config;

		eprosima::fastdds::dds::Topic* addTopic(dls::topicType topicData_, bool add_ros2_namespace = true);
		eprosima::fastdds::dds::Topic* addTopicFromDatabase(std::string topicName, bool add_ros2_namespace = true);

		std::multimap<std::string, eprosima::fastdds::rtps::GUID_t> discovered_participants_info;

		/*!
		* @brief Custom Callback on_participant_discovery
		* @param[in] participant domain participant discovering a new domain participant
		* @param[in] info information about the discovered domain participant
		*/
		void on_participant_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastdds::rtps::ParticipantDiscoveryStatus status,
            const eprosima::fastdds::dds::ParticipantBuiltinTopicData& info,
            bool& should_be_ignored) override;

		void on_data_writer_discovery(
			eprosima::fastdds::dds::DomainParticipant* participant,
			eprosima::fastdds::rtps::WriterDiscoveryStatus status,
			const eprosima::fastdds::dds::PublicationBuiltinTopicData& info,
			bool& should_be_ignored) override;

		void on_data_reader_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastdds::rtps::ReaderDiscoveryStatus status,
            const eprosima::fastdds::dds::SubscriptionBuiltinTopicData& info,
            bool& should_be_ignored) override;

		bool is_type_registered_in_xml_(const std::string& type_name);
		bool is_type_registered_in_factory_(const std::string& type_name);

		eprosima::fastdds::dds::DynamicType::_ref_type get_type_registered_(const std::string& type_name);
	};

} // namespace dls
/// \endcond
#endif // DDSPARTICIPANT_HPP
