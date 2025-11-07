#include "dls2/util/messaging/dds_participant.hpp"

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilder.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilderFactory.hpp>

#include <fastdds/rtps/transport/UDPv4TransportDescriptor.hpp>

/// \cond doxygen_namespace_dls
namespace dls
{

	DDSParticipant::DDSParticipant(std::string partName_, dls::domainType domain_, eprosima::fastdds::rtps::DiscoveryProtocol part_type)
    	: participant(nullptr)
        , publisher(nullptr)
        , subscriber(nullptr)
		, topicListener(nullptr)
		, config(DDSParticipant::getServersConfig())
	{
		eprosima::fastdds::dds::DomainParticipantQos participantQos;
		participantQos.wire_protocol().builtin.mutation_tries = 250u; //limit discoverable data readers/writers (default is 100u)
		participantQos.properties().properties().emplace_back("fastdds.type_propagation","enabled"); // it is enabled by default

		// Get server info from the yaml file
		server_ip = config[dls::domains::id_to_name_map[domain_]]["ip"].as<std::string>();
		server_port = config[dls::domains::id_to_name_map[domain_]]["port"].as<int>();

		// Define server locator
		eprosima::fastdds::rtps::Locator_t server_locator;
		eprosima::fastdds::rtps::IPLocator::setIPv4(server_locator, server_ip);
		server_locator.port = server_port;
		server_locator.kind = LOCATOR_KIND_UDPv4;

		// participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastdds::rtps::DiscoveryProtocol::SIMPLE;
		// Set participant QoS depending on if it is a CLIENT, a SERVER or a SUPER CLIENT
		participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = part_type;
		if(	part_type == eprosima::fastdds::rtps::DiscoveryProtocol::CLIENT ||
			part_type == eprosima::fastdds::rtps::DiscoveryProtocol::SUPER_CLIENT) // -- Configure the participant as CLIENT or SUPER_CLIENT
		{
			// -- Connect to the remote server
			participantQos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(server_locator);
		}
		else if (part_type == eprosima::fastdds::rtps::DiscoveryProtocol::SERVER) // -- Configure the participant as SERVER
		{
			// -- Add the server locator to the metatraffic uncast locator list
			participantQos.wire_protocol().builtin.metatrafficUnicastLocatorList.push_back(server_locator);
		}

		// participantQos.wire_protocol().builtin.discovery_config.initial_announcements.count = 0;
		// participantQos.wire_protocol().builtin.discovery_config.initial_announcements.period = Duration_t(0, 100000000u);
		
		participantQos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastdds::dds::Duration_t(3, 1);
        participantQos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = eprosima::fastdds::dds::Duration_t(1, 2);
		participantQos.name(partName_);

		participant_name = partName_;

		// Create a UDP descriptor for the new transport.
		auto udp_transport = std::make_shared<eprosima::fastdds::rtps::UDPv4TransportDescriptor>();
		// udp_transport->sendBufferSize = 9216;
		// udp_transport->receiveBufferSize = 9216;
		// udp_transport->non_blocking_send = true; // it avoids to wait for the available space in the UDP socket buffer
		// Link the Transport Layer to the Participant.
		participantQos.transport().user_transports.push_back(udp_transport);
		// Avoid using the default transport (i.e. SHM)
		participantQos.transport().use_builtin_transports = false;

		if(ENABLE_FASTDDS_STATISTICS)
		{
			participantQos.properties().properties().emplace_back("fastdds.statistics",
            "HISTORY_LATENCY_TOPIC;" \
            "NETWORK_LATENCY_TOPIC;" \
            "PUBLICATION_THROUGHPUT_TOPIC;" \
            "SUBSCRIPTION_THROUGHPUT_TOPIC;" \
            "RTPS_SENT_TOPIC;" \
            "RTPS_LOST_TOPIC;" \
            "HEARTBEAT_COUNT_TOPIC;" \
            "ACKNACK_COUNT_TOPIC;" \
            "NACKFRAG_COUNT_TOPIC;" \
            "GAP_COUNT_TOPIC;" \
            "DATA_COUNT_TOPIC;" \
            "RESENT_DATAS_TOPIC;" \
            "SAMPLE_DATAS_TOPIC;" \
            "PDP_PACKETS_TOPIC;" \
            "EDP_PACKETS_TOPIC;" \
            "DISCOVERY_TOPIC;" \
            "PHYSICAL_DATA_TOPIC");
		}

		eprosima::fastdds::dds::StatusMask mask;

		this->participant = eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->create_participant(
			domain_,
			participantQos,
			this,
			mask.none()
		);

		if (!this->participant)
			throw std::runtime_error("Error: could not create participant");

		// create publisher
		this->publisher = this->participant->create_publisher(
			eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT,
			nullptr);

		if (this->publisher == nullptr)
		{
			throw std::runtime_error("Error: could not create publisher");
		}

		// create subscriber
		this->subscriber = this->participant->create_subscriber(
			eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT,
			nullptr);

		if (this->subscriber == nullptr)
		{
			throw std::runtime_error("Error: could not create subscriber");
		}
	}

	DDSParticipant::~DDSParticipant()
	{


		if (!this->participant){
			return;
		}


		// delete all data writers and data readers
		if (this->publisher->delete_contained_entities() !=eprosima::fastdds::dds::RETCODE_OK)
		{
			std::cout << "CANNOT DELETE PUBLISHER CONTAINED ENTITIES FOR THE PARTICIPANT " << participant_name << std::endl;
		}
		if (this->subscriber->delete_contained_entities() !=eprosima::fastdds::dds::RETCODE_OK)
		{
			std::cout << "CANNOT DELETE SUBSCRIBER CONTAINED ENTITIES FOR THE PARTICIPANT " << participant_name << std::endl;
		}

		// delete publisher
		if (this->publisher != nullptr)
		{
			if (this->participant->delete_publisher(this->publisher) !=eprosima::fastdds::dds::RETCODE_OK)
			{
				std::cout << "CANNOT DELETE PUBLISHER OF THE PARTICIPANT " << participant_name << std::endl;
			}
		}

		// delete subscriber
		if (this->subscriber != nullptr)
		{
			if (this->participant->delete_subscriber(this->subscriber) !=eprosima::fastdds::dds::RETCODE_OK)
			{
				std::cout << "CANNOT DELETE SUBSCRIBER OF THE PARTICIPANT " << participant_name << std::endl;
			}
		}

		for (auto elem : this->topics)
		{
			if (elem.second != nullptr)
			{
				if(this->participant->delete_topic(elem.second) !=eprosima::fastdds::dds::RETCODE_OK)
				{
					std::cout << "CANNOT REMOVE TOPIC " << elem.first << " OF THE PARTICIPANT " << participant_name << std::endl;
				}
			}
		}
		// delete participant
		eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->delete_participant(this->participant);
	}

	eprosima::fastdds::dds::DataWriter* DDSParticipant::getWriter(std::string name)
	{
		auto ret = this->writers.find(name);
		if(ret != this->writers.end())
			return ret->second;

		return nullptr;
	}

	eprosima::fastdds::dds::DataReader* DDSParticipant::getReader(std::string name)
	{
		auto ret = this->readers.find(name);
		if(ret != this->readers.end())
			return ret->second;

		return nullptr;
	}

	std::shared_ptr<dls::DDSSubListener> DDSParticipant::getSubListener(const std::string& name)
	{
		auto ret = this->subListeners.find(name);
		if(ret != this->subListeners.end())
			return ret->second;

		throw std::runtime_error("No listener of for the reader " + name + "is found. Returning null pointer.");

		return nullptr;
	}

	std::shared_ptr<dls::DDSPubListener> DDSParticipant::getPubListener(const std::string& name)
	{
		auto ret = this->pubListeners.find(name);
		if(ret != this->pubListeners.end())
			return ret->second;

		throw std::runtime_error("No listener for the writer " + name + "is found. Returning null pointer.");

		return nullptr;
	}

	eprosima::fastdds::dds::DataWriter* DDSParticipant::addWriter(
		std::string writerName_,
		dls::topicType topicData_,
		eprosima::fastdds::dds::DataWriterQos qos,
		bool add_ros2_namespace)
	{
		if(this->writers.find(writerName_) != this->writers.end())
			throw std::runtime_error("THE WRITER " + writerName_ + " ALREADY EXISTS, YOU ARE TRYING TO CREATE TWICE");

		auto topic = this->addTopic(topicData_, add_ros2_namespace);

		if (topic == nullptr)
			throw std::runtime_error("THE WRITER " + writerName_ + " COULDN'T CREATE THE TOPIC " + topicData_.first);
		std::shared_ptr<dls::DDSPubListener> listener = std::make_shared<DDSPubListener>();

		auto writer = this->publisher->create_datawriter(
			topic,
			qos,
			listener.get()
			//&this->publisher_listener
		);

		if (writer == nullptr)
			throw std::runtime_error("THE WRITER " + writerName_ + " COULDN'T BE CREATED");

		this->writers.insert({writerName_, writer});
		this->pubListeners.insert({writerName_, listener});

		return writer;
	}

	bool DDSParticipant::deleteWriter(const std::string& writer_name)
	{
		auto writer = this->getWriter(writer_name);

		if(writer == nullptr){
			std::cout <<  "THE WRITER " << writer_name << " DOES NOT BELONG TO THE SUBSCRIBER" << std::endl;
			return false;
		}

		eprosima::fastdds::dds::ReturnCode_t result (this->publisher->delete_datawriter(writer));

		if(result ==eprosima::fastdds::dds::RETCODE_ERROR)
		{
			std::cout <<  "RETCODE_ERROR ERROR WHEN REMOVING THE WRITER " << writer_name << std::endl;
		}

		this->writers.erase(writer_name);
		return true;
	}

	eprosima::fastdds::dds::DataReader *DDSParticipant::addReader(
		std::string readerName_,
		dls::topicType topicData_,
		std::function<void(void *)> callback_,
		eprosima::fastdds::dds::DataReaderQos qos,
		bool add_ros2_namespace)
	{
		if(this->readers.find(readerName_) != this->readers.end())
		{
			std::cout << "THE READER " << readerName_ << " ALREADY EXISTS, YOU ARE TRYING TO CREATE TWICE" << std::endl;
			return this->readers.find(readerName_)->second;
		}

		auto topic = this->addTopic(topicData_,add_ros2_namespace);

		// error could not add topic
		if (topic == nullptr)
			return nullptr;

		std::shared_ptr<dls::DDSSubListener> listener = std::make_shared<DDSSubListener>(callback_);

		auto reader = this->subscriber->create_datareader(
			topic,
			qos,
			listener.get());

		if (reader != nullptr)
		{

			this->readers.insert({readerName_, reader});
			this->subListeners.insert({readerName_, listener});
		}

		return reader;
	}

	eprosima::fastdds::dds::Topic * DDSParticipant::getTopicFromString(const std::string& topic_name){

		eprosima::fastdds::dds::Topic * found_topic = this->topics.find(topic_name)->second;

		return found_topic;

	}

	std::string DDSParticipant::getTypeNameFromTopic(const std::string& topic_name)
	{
		std::string type_name = discovery_database[topic_name].get_type_name();

		return type_name;
	}

	bool DDSParticipant::topicFound(const std::string& topic_name){

		if (discovery_database.find(topic_name) != discovery_database.end()){
			return true;
		}

		return false;

	}



	bool DDSParticipant::deleteReader(const std::string& reader_name)
	{
		auto reader = this->getReader(reader_name);

		if(reader == nullptr)
		{
			std::cout <<  "THE READER " << reader_name << " DOES NOT BELONG TO THE SUBSCRIBER" << std::endl;
			return false;
		}

		eprosima::fastdds::dds::ReturnCode_t result (this->subscriber->delete_datareader(reader));

		if(result ==eprosima::fastdds::dds::RETCODE_ERROR)
		{
			std::cout <<  "RETCODE_ERROR ERROR WHEN REMOVING THE READER " << reader_name << std::endl;
		}

		this->readers.erase(reader_name);
		return true;
	}

	eprosima::fastdds::dds::Topic *DDSParticipant::addTopic(dls::topicType topicData_, bool add_ros2_namespace)
	{
		// add ROS2 namespace
		if(add_ros2_namespace)
			topicData_.first = std::string("rt/")+topicData_.first;

		if(!this->participant)
			return nullptr;

		auto search = this->topics.find(topicData_.first);

		if(search != topics.end())
		{
			return search->second;
		}

		if(!this->participant->find_type(topicData_.second.get_type_name()))
		{
			this->participant->register_type(topicData_.second);
		}
		auto topic = this->participant->create_topic(
			topicData_.first,
			topicData_.second.get_type_name(),
			eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);

		if(topic == nullptr)
			throw std::runtime_error("Error: could not create publisher topic");

		this->topics.insert({topicData_.first, topic});

		return topic;
	}

	eprosima::fastdds::dds::Topic *DDSParticipant::addTopicFromDatabase(std::string topicName, bool add_ros2_namespace)
	{
		// add ROS2 namespace
		if(add_ros2_namespace)
			topicName = std::string("rt/")+topicName;

		if(!this->participant){
			return nullptr;

		}

		// check if topic was already added
		auto search = this->topics.find(topicName);
		if(search != topics.end()){
			return search->second;
		}

		// check if topic was discovered
		if(discovery_database.find(topicName) == discovery_database.end())
		{
			std::cout << "THE TOPIC " << topicName << " IS NOT DISCOVERED."<< this->getName() <<".addTopicFromDatabase(topicName) function is returning nullptr." << std::endl;
			return nullptr;
		}

		// the topic was discovered, so it is stored its type support. The topic can be created
		auto topic = this->addTopic(dls::topicType(topicName, discovery_database[topicName]),false);

		return topic;
	}

	std::vector<std::string> DDSParticipant::getDiscoveredTopics(){
		std::vector <std::string> topicList;
		for(auto & it: discovery_database){
			topicList.emplace_back(it.first);
		}
		return topicList;
	}

	std::unordered_map<std::string, eprosima::fastdds::dds::TypeSupport> DDSParticipant::get_discovery_database()
	{
		return discovery_database;
	}

	std::unordered_map<std::string, eprosima::fastdds::dds::DynamicType::_ref_type> DDSParticipant::get_discovery_database_dyn_types()
	{
		return discovery_database_dyn_types;
	}

	std::vector<std::string> DDSParticipant::getParticipants()
	{
		if(!this->participant)
			return std::vector<std::string>(0);

		return this->participant->get_participant_names();
	}

	std::multimap<std::string, eprosima::fastdds::rtps::GUID_t> DDSParticipant::getDiscoveredParticipantsInfo()
	{
		return this->discovered_participants_info;
	}

	bool DDSParticipant::sendMessage(std::string writerName, void *msg)
	{
		auto writer = this->writers.find(writerName);
		if(writer == this->writers.end())
		{
			std::cout << "WRITER " << writerName << " DOES NOT EXISTS" << std::endl;
			return false;
		}

		return writer->second->write(msg);
	}

	void DDSParticipant::on_participant_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastdds::rtps::ParticipantDiscoveryStatus status,
            const eprosima::fastdds::dds::ParticipantBuiltinTopicData& info,
            bool& should_be_ignored)
	{
		should_be_ignored = false;
		static_cast<void>(participant);
		if (status == eprosima::fastdds::rtps::ParticipantDiscoveryStatus::DISCOVERED_PARTICIPANT)
		{
			// std::cout << this->participant->get_qos().name() <<": New participant discovered: " << info.participant_name << ", current num. disc. DPs: " << discovered_participants_info.size()<< std::endl;
			discovered_participants_info.insert({static_cast<std::string>(info.participant_name), info.guid});
		}
		else if (status == eprosima::fastdds::rtps::ParticipantDiscoveryStatus::DROPPED_PARTICIPANT)
		{
			// std::cout << this->participant->get_qos().name() <<": Participant is dropped: " << info.participant_name << std::endl;
			discovered_participants_info.erase(static_cast<std::string>(info.participant_name));
		}
		else if (status == eprosima::fastdds::rtps::ParticipantDiscoveryStatus::REMOVED_PARTICIPANT)
		{
			// std::cout << this->participant->get_qos().name() <<": Participant is removed: " << info.participant_name << std::endl;
			discovered_participants_info.erase(static_cast<std::string>(info.participant_name));
		}
	}

	void DDSParticipant::on_data_writer_discovery(
			eprosima::fastdds::dds::DomainParticipant* participant,
			eprosima::fastdds::rtps::WriterDiscoveryStatus status,
			const eprosima::fastdds::dds::PublicationBuiltinTopicData& info,
			bool& should_be_ignored)
	{
		should_be_ignored = false;

		// warning suppress
		(void)participant;

		// For any new discovered topic, its type support is dynamically created and stored
		if (status == eprosima::fastdds::rtps::WriterDiscoveryStatus::DISCOVERED_WRITER &&
			discovery_database.find(info.topic_name.to_string()) == discovery_database.end())
		{
			// Get remote type information
			eprosima::fastdds::dds::xtypes::TypeObject remote_type_object;
			if (eprosima::fastdds::dds::RETCODE_OK != eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_object(
						info.type_information.type_information.complete().typeid_with_size().type_id(),
						remote_type_object))
			{
				std::cout << "Cannot get the remote type information" << std::endl;
				return;
			}

			// Reconstruct the remote type
			eprosima::fastdds::dds::DynamicType::_ref_type remote_type = eprosima::fastdds::dds::DynamicTypeBuilderFactory::get_instance()->create_type_w_type_object(
				remote_type_object)->build();
			eprosima::fastdds::dds::TypeSupport dyn_type_support(new eprosima::fastdds::dds::DynamicPubSubType(remote_type));

			// Save the type support in the discovery database
			discovery_database[info.topic_name.to_string()] = dyn_type_support;
			discovery_database_dyn_types[info.topic_name.to_string()] = remote_type;
		}
	}

	void DDSParticipant::on_data_reader_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastdds::rtps::ReaderDiscoveryStatus status,
            const eprosima::fastdds::dds::SubscriptionBuiltinTopicData& info,
            bool& should_be_ignored)
	{
		should_be_ignored = false;
		// warning suppress
		(void)participant;
				// Only set as new topic discovered if it is ALIVE
		if (status == eprosima::fastdds::rtps::ReaderDiscoveryStatus::DISCOVERED_READER)
		{
			// Get Topic of DataReader discovered and set it as discovered
			std::string topic_name = info.topic_name.to_string();
			std::string type_name = info.type_name.to_string();

			// Set Topic as discovered. If it is not new nothing happen
			// if(DDSParticipant::is_type_registered_in_participant_(type_name))
				// on_topic_discovery_(topic_name, type_name);
		}

	}

	void DDSParticipant::setTopicListener(dls::DDSPartListener *listener_)
	{
		this->topicListener = listener_;
	}

	std::string DDSParticipant::getName() const{
		return participant_name;
	}

	YAML::Node DDSParticipant::getServersConfig(){
		char * val;                                                                        
		val = getenv("DLS_SERVERS_PATH");                                                       
		std::string servers_path = "";                                                           
		if (val != NULL) {                                                                 
			servers_path = val;                                                                    
		}
		else{
			servers_path = "/usr/include/dls2/util/messaging/servers.yaml";
		}
		return YAML::LoadFile(servers_path);
	}
} // namespace dls