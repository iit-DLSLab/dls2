#ifndef DDSPARTICIPANT_CPP
#define DDSPARTICIPANT_CPP

#include "dls2/util/messaging/dds_participant.hpp"
#include <fastrtps/xmlparser/XMLProfileManager.h>
#include <fastrtps/types/TypeObjectFactory.h>

#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>

/// \cond doxygen_namespace_dls
namespace dls
{

	DDSParticipant::DDSParticipant(std::string partName_, dls::domainType domain_, eprosima::fastrtps::rtps::DiscoveryProtocol_t part_type, bool tupelookup_server)
    	: participant(nullptr)
        , publisher(nullptr)
        , subscriber(nullptr)
		, topicListener(nullptr)
		, config(YAML::LoadFile("/usr/include/dls2/util/messaging/servers.yaml"))
	{
		eprosima::fastdds::dds::DomainParticipantQos participantQos;
		if (tupelookup_server)
		{
			participantQos.wire_protocol().builtin.typelookup_config.use_server = true;	
		}
		else
		{
			participantQos.wire_protocol().builtin.typelookup_config.use_client = true;
		}

		// Get server info from the yaml file
		server_ip = config[domain_]["ip"].as<std::string>();
		server_port = config[domain_]["port"].as<double>();
		server_guid_prefix = config[domain_]["guid_prefix"].as<std::string>();

		// Define server locator
		eprosima::fastrtps::rtps::Locator_t server_locator;
		eprosima::fastrtps::rtps::IPLocator::setIPv4(server_locator, server_ip);
		eprosima::fastrtps::rtps::IPLocator::setPhysicalPort(server_locator, server_port);
		server_locator.kind = LOCATOR_KIND_UDPv4;

		// participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;
		// Set participant QoS depending on if it is a CLIENT, a SERVER or a SUPER CLIENT
		participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = part_type;
		if(	part_type == eprosima::fastrtps::rtps::DiscoveryProtocol_t::CLIENT ||
			part_type == eprosima::fastrtps::rtps::DiscoveryProtocol_t::SUPER_CLIENT) // -- Configure the participant as CLIENT or SUPER_CLIENT
		{
			// -- Add the server locator in the metatraffic unicast locator list of the remote server attributes
			eprosima::fastrtps::rtps::RemoteServerAttributes remote_server_attr;
			remote_server_attr.metatrafficUnicastLocatorList.push_back(server_locator);
			// -- Set the GUID prefix to identify the server
			remote_server_attr.ReadguidPrefix(server_guid_prefix.c_str());
			// -- Connect to the remote server
			participantQos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(remote_server_attr);
		}
		else if (part_type == eprosima::fastrtps::rtps::DiscoveryProtocol_t::SERVER) // -- Configure the participant as SERVER
		{
			// -- Add the server locator to the metatraffic uncast locator list
			participantQos.wire_protocol().builtin.metatrafficUnicastLocatorList.push_back(server_locator);
			// -- Set the GUID prefix to identify this server
    		std::istringstream(server_guid_prefix) >> participantQos.wire_protocol().prefix;
		}

		participantQos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::Duration_t(3, 1);
        participantQos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = eprosima::fastrtps::Duration_t(1, 2);
		participantQos.name(partName_);

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
		if (!this->participant)
			return;

		const char* str=this->participant->get_qos().name();
		const std::string participant_name = str;
		// delete all data writers and data readers
		if (this->publisher->delete_contained_entities() != ReturnCode_t::RETCODE_OK)
		{
			std::cout << "CANNOT DELETE PUBLISHER CONTAINED ENTITIES FOR THE PARTICIPANT " << participant_name << std::endl;
		}
		if (this->subscriber->delete_contained_entities() != ReturnCode_t::RETCODE_OK)
		{
			std::cout << "CANNOT DELETE SUBSCRIBER CONTAINED ENTITIES FOR THE PARTICIPANT " << participant_name << std::endl;
		}
		
		// delete publisher
		if (this->publisher != nullptr)
		{
			if (this->participant->delete_publisher(this->publisher) != ReturnCode_t::RETCODE_OK)
			{
				std::cout << "CANNOT DELETE PUBLISHER OF THE PARTICIPANT " << participant_name << std::endl;
			}
		}

		// delete subscriber
		if (this->subscriber != nullptr)
		{
			if (this->participant->delete_subscriber(this->subscriber) != ReturnCode_t::RETCODE_OK)
			{
				std::cout << "CANNOT DELETE SUBSCRIBER OF THE PARTICIPANT " << participant_name << std::endl;
			}
		}

		for (auto elem : this->topics)
		{
			if (elem.second != nullptr)
			{
				if(this->participant->delete_topic(elem.second) != ReturnCode_t::RETCODE_OK)
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

		throw std::runtime_error("No listener of for the writer " + name + "is found. Returning null pointer.");

		return nullptr;
	}

	eprosima::fastdds::dds::DataWriter* DDSParticipant::addWriter(
		std::string writerName_,
		dls::topicType topicData_,
		eprosima::fastdds::dds::DataWriterQos qos)
	{
		if(this->writers.find(writerName_) != this->writers.end())
			throw std::runtime_error("THE WRITER " + writerName_ + " ALREADY EXISTS, YOU ARE TRYING TO CREATE TWICE");

		auto topic = this->addTopic(topicData_);

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
		
		ReturnCode_t result (this->publisher->delete_datawriter(writer));

		if(result == ReturnCode_t::RETCODE_ERROR)
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
		eprosima::fastdds::dds::DataReaderQos qos)
	{
		if(this->readers.find(readerName_) != this->readers.end())
		{
			std::cout << "THE READER " << readerName_ << " ALREADY EXISTS, YOU ARE TRYING TO CREATE TWICE" << std::endl;
			return this->readers.find(readerName_)->second;
		}		

		auto topic = this->addTopic(topicData_);

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

	eprosima::fastdds::dds::DataReader* DDSParticipant::addReader(
		std::string readerName_,
		std::string topicName,
		std::function<void(void *)> callback_,
		eprosima::fastdds::dds::DataReaderQos qos)
	{
		if(this->readers.find(readerName_) != this->readers.end())
		{
			std::cout << "THE READER " << readerName_ << " ALREADY EXISTS, YOU ARE TRYING TO CREATE TWICE" << std::endl;
			return this->readers.find(readerName_)->second;
		}	
		
		auto topic = this->addTopic(topicName); //this->getTopicFromString(topicName); //


		std::cout << "Reader to be created" << std::endl;
		// error could not add topic
		if (topic == nullptr){
			std::cout << "nullptr for topic" << std::endl;
			return nullptr;

		}
			

		std::shared_ptr<dls::DDSSubListener> listener = std::make_shared<DDSSubListener>(callback_);
		std::cout << "listener made" << std::endl;

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
		
		ReturnCode_t result (this->subscriber->delete_datareader(reader));

		if(result == ReturnCode_t::RETCODE_ERROR)
		{
			std::cout <<  "RETCODE_ERROR ERROR WHEN REMOVING THE READER " << reader_name << std::endl;
		}
		
		this->readers.erase(reader_name);
		return true;
	}

	eprosima::fastdds::dds::Topic *DDSParticipant::addTopic(dls::topicType topicData_)
	{
		if(!this->participant)
			return nullptr;

		std::cout << "Adding in the topic: " << topicData_.first << std::endl;
		std::cout << "\t type name is: " << topicData_.second.get_type_name() << std::endl;
		auto search = this->topics.find(topicData_.first);

		if(search != topics.end())
			return search->second;

		if(!this->participant->find_type(topicData_.second.get_type_name()))
		{
			topicData_.second->auto_fill_type_information(false);
    		topicData_.second->auto_fill_type_object(true);
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

	eprosima::fastdds::dds::Topic *DDSParticipant::addTopic(std::string topicName)
	{
		if(!this->participant)
			return nullptr;

		auto search = this->topics.find(topicName);
		std::string type_name = discovery_database[topicName];

		std::cout << "Type " << topicName << " being added is: " << type_name << std::endl;

		if(search != topics.end()){
			std::cout << "topic found" << std::endl;
			return search->second;
		}

			

		if(!this->participant->find_type(topicName))
		{
			std::cout << "Couldnt find the topic type, so adding it ourselves" << std::endl;
			// topicData_.second->auto_fill_type_information(false);
    		// topicData_.second->auto_fill_type_object(true);
			// this->participant->register_type(topicData_.second);
		}

		auto topic = this->participant->create_topic(
			topicName,
			type_name,
			eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);

		std::cout << "Topic has been created" << std::endl;
		if(topic == nullptr)
			throw std::runtime_error("Error: could not create publisher topic");

		this->topics.insert({topicName, topic});

		return topic;
	}


	std::vector<std::string> DDSParticipant::getParticipants()
	{
		if(!this->participant)
			return std::vector<std::string>(0);

		return this->participant->get_participant_names();
	}

	std::multimap<std::string, eprosima::fastrtps::rtps::GUID_t> DDSParticipant::getDiscoveredParticipantsInfo()
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
            eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info)
	{
		static_cast<void>(participant);
		if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERY_STATUS::DISCOVERED_PARTICIPANT)
		{
			discovered_participants_info.insert({static_cast<std::string>(info.info.m_participantName), info.info.m_guid});
			// std::cout << " Discovered a new participant:" << static_cast<std::string>(info.info.m_participantName) << std::endl;

		}
		else if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERY_STATUS::REMOVED_PARTICIPANT)
		{
			discovered_participants_info.erase(static_cast<std::string>(info.info.m_participantName));
		}
	}

	void DDSParticipant::on_publisher_discovery(
        eprosima::fastdds::dds::DomainParticipant* participant,
        eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info)
	{
		// warning suppress
		(void)participant;

		// Only set as new topic discovered if it is ALIVE
		if (info.status == eprosima::fastrtps::rtps::WriterDiscoveryInfo::DISCOVERY_STATUS::DISCOVERED_WRITER)
		{
			// Get Topic of DataWriter discovered and set it as discovered
			std::string topic_name = info.info.topicName().to_string();
			std::string type_name = info.info.typeName().to_string();

			// std::cout << " Discovered a new topic: " << topic_name << " of type: " << type_name << std::endl;
			// Set Topic as discovered. If it is not new nothing happen
			if(DDSParticipant::is_type_registered_in_participant_(type_name))
				on_topic_discovery_(topic_name, type_name);
		}
	}

	void DDSParticipant::on_type_information_received(
        eprosima::fastdds::dds::DomainParticipant*,
        const eprosima::fastrtps::string_255 topic_name,
        const eprosima::fastrtps::string_255 type_name,
        const eprosima::fastrtps::types::TypeInformation& type_information)
	{
		std::cout << "received the type information for: " << topic_name.to_string() << std::endl;
		if(!this->participant)
			return;

		// Prepare callback that will be executed after registering type
		std::function<void(const std::string&, const eprosima::fastrtps::types::DynamicType_ptr)> callback(
			[this, topic_name]
				(const std::string&, const eprosima::fastrtps::types::DynamicType_ptr type)
			{
				this->on_topic_discovery_(topic_name.to_string(), type->get_name());
			});
	
		if(DDSParticipant::is_type_registered_in_participant_(type_name.to_string()))
			return;
		
		// Registering type and creating reader
		this->participant->register_remote_type(
			type_information,
			type_name.to_string(),
			callback);
	}

	void DDSParticipant::on_type_discovery(
			eprosima::fastdds::dds::DomainParticipant* participant,
			const eprosima::fastrtps::rtps::SampleIdentity& request_sample_id,
			const eprosima::fastrtps::string_255& topic,
			const eprosima::fastrtps::types::TypeIdentifier* identifier,
			const eprosima::fastrtps::types::TypeObject* object,
			eprosima::fastrtps::types::DynamicType_ptr dyn_type)
	{
		static_cast<void>(participant); // remove compilation warnings
		static_cast<void>(request_sample_id); // remove compilation warnings
		static_cast<void>(identifier); // remove compilation warnings
		static_cast<void>(object); // remove compilation warnings
		// Create TypeSupport and register it
		eprosima::fastdds::dds::TypeSupport(
			new eprosima::fastrtps::types::DynamicPubSubType(dyn_type)).register_type(participant);

		// In case this callback is sent, it means that the type is already registered, so notify
		// TODO in future it would be better to update every topic in this type name, and not just the one calling here
		on_topic_discovery_(topic.to_string(), dyn_type->get_name());
	}

	void DDSParticipant::on_topic_discovery_(const std::string& topic_name, const std::string& type_name)
	{
		// Check if this topic has already been discovered
		if (discovery_database.find(topic_name) != discovery_database.end())
			return;

		discovery_database[topic_name] = type_name;

		// Call listener callback to notify new topic
		if (this->topicListener)
		{
			this->topicListener->on_topic_discovery(topic_name, type_name);
		}
	}

	void DDSParticipant::setTopicListener(dls::DDSPartListener *listener_)
	{
		this->topicListener = listener_;
	}


	bool DDSParticipant::is_type_registered_in_participant_(const std::string& type_name)
	{

		if (!this->participant)
			return false;

		if (this->participant->find_type(type_name))
			return true;

		// It may happen that type is registered in XML and not in Participant
		// If so, register it in Participant
		if (is_type_registered_in_xml_(type_name))
		{
			// Create TypeSupport and register it
			eprosima::fastdds::dds::TypeSupport(
				new eprosima::fastrtps::types::DynamicPubSubType(
					get_type_registered_(type_name))).register_type(this->participant);
			return true;
		}

		// It could also be in TypeObjectFactory because it has been registered by other Participant (a previous one)
		// and still be stored in the singleton
		if (is_type_registered_in_factory_(type_name))
		{
			// Create TypeSupport and register it
			eprosima::fastdds::dds::TypeSupport(
				new eprosima::fastrtps::types::DynamicPubSubType(
					get_type_registered_(type_name))).register_type(this->participant);
			return true;
		}

		return false;
	}

	bool DDSParticipant::is_type_registered_in_xml_(
			const std::string& type_name)
	{
		return nullptr != eprosima::fastrtps::xmlparser::XMLProfileManager::getDynamicTypeByName(type_name);
	}

	bool DDSParticipant::is_type_registered_in_factory_(
			const std::string& type_name)
	{
		return nullptr !=  eprosima::fastrtps::types::TypeObjectFactory::get_instance()->get_type_object(type_name, true);
	}

	eprosima::fastrtps::types::DynamicType_ptr DDSParticipant::get_type_registered_(
        const std::string& type_name)
	{
		// Get DynamicType builder
		auto builder = eprosima::fastrtps::xmlparser::XMLProfileManager::getDynamicTypeByName(type_name);

		// If not builder associated, the type does not exist
		if (!builder)
		{
			// Check if it could be generated
			// This case is when it has not been registered by XML
			auto type_object =
					eprosima::fastrtps::types::TypeObjectFactory::get_instance()->get_type_object(type_name,
							true);
			if (!type_object)
			{
				throw std::runtime_error("Dynamic type not registered");
			}

			auto type_id =
					eprosima::fastrtps::types::TypeObjectFactory::get_instance()->get_type_identifier(type_name,
							true);
			if (!type_id)
			{
				throw std::runtime_error("Dynamic type not registered");
			}

			auto dyn_type = eprosima::fastrtps::types::TypeObjectFactory::get_instance()->build_dynamic_type(type_name,
							type_id,
							type_object);

			return dyn_type;
		}
		else
		{
			return builder->build();
		}
	}

} // namespace dls

#endif /* end of include guard: DDSPARTICIPANT_CPP */