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
#ifndef DDSPARTICIPANT_CPP
#define DDSPARTICIPANT_CPP

#include "dls2/util/messaging/dds_participant.hpp"

/// \cond doxygen_namespace_dls
namespace dls
{

	DDSParticipant::DDSParticipant(
		std::string partName_,
		dls::domainType domain_)
		: participant(nullptr), publisher(nullptr), subscriber(nullptr)
	{
		eprosima::fastdds::dds::DomainParticipantQos participantQos;
		participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;
		participantQos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = eprosima::fastrtps::Duration_t(1, 2);
		participantQos.name(partName_);

		this->participant = eprosima::fastdds::dds::DomainParticipantFactory::
								get_instance()
									->create_participant(domain_, participantQos);

		if (this->participant == nullptr)
		{
			throw std::runtime_error("Error: could not create participant");
		}

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
		// delete all data writers and data readers
		this->publisher->delete_contained_entities();
		this->subscriber->delete_contained_entities();

		// delete publisher
		if (this->publisher != nullptr)
			this->participant->delete_publisher(this->publisher);

		// delete subscriber
		if (this->subscriber != nullptr)
			this->participant->delete_subscriber(this->subscriber);

		for (auto elem : this->topics)
			if (elem.second != nullptr)
				this->participant->delete_topic(elem.second);

		for (auto elem : this->subListeners)
		{
			delete elem;
		}

		// delete participant
		eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->delete_participant(this->participant);
	}

	eprosima::fastdds::dds::DataWriter *DDSParticipant::addWriter(
		std::string writerName_,
		dls::topicType topicData_)
	{
		auto topic = this->addTopic(topicData_);

		if (topic == nullptr)
			return nullptr;

		auto writer = this->publisher->create_datawriter(
			topic,
			eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT,
			nullptr
			//&this->publisher_listener
		);

		if (writer != nullptr)
			this->writers.insert({writerName_, writer});

		return writer;
	}

	eprosima::fastdds::dds::DataReader *DDSParticipant::addReader(
		std::string readerName_,
		dls::topicType topicData_,
		std::function<void(void *)> callback_)
	{
		auto topic = this->addTopic(topicData_);

		// error could not add topic
		if (topic == nullptr)
			return nullptr;

		DDSSubListener *listener = new DDSSubListener(callback_);

		auto reader = this->subscriber->create_datareader(
			topic,
			eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT,
			listener);

		if (reader != nullptr)
		{
			this->readers.insert({readerName_, reader});
			this->subListeners.push_back(listener);
		}

		return reader;
	}

	eprosima::fastdds::dds::Topic *DDSParticipant::addTopic(dls::topicType topicData_)
	{

		auto search = this->topics.find(topicData_.first);

		if (search != topics.end())
			return search->second;

		if (!this->types.contains(topicData_.second.get_type_name())){
			this->types.insert(topicData_.second.get_type_name());
			this->participant->register_type(topicData_.second);
		}

		auto topic = this->participant->create_topic(
			topicData_.first,
			topicData_.second.get_type_name(),
			eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);

		if (topic == nullptr)
		{
			// throw std::runtime_error(
			// 	"Error: could not create publisher topic"
			// );
		}

		this->topics.insert({topicData_.first, topic});

		return topic;
	}

	std::vector<std::string> DDSParticipant::getParticipants()
	{
		return this->participant->get_participant_names();
	}

	void DDSParticipant::sendMessage(std::string writerName, void *msg)
	{
		this->writers.find(writerName)->second->write(msg);
	}

} // namespace dls
/// \endcond

#endif /* end of include guard: DDSPARTICIPANT_CPP */