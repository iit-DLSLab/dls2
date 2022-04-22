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
#ifndef PARTICIPANT_CPP
#define PARTICIPANT_CPP

#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/participant/Participant.h>

#include "dls2/util/messaging/participant.hpp"


/// \cond doxygen_namespace_dls
namespace dls
{

	DDSParticipant::DDSParticipant(
		std::string 	partName_,
		dls::domainType	domain_
	)
		: participant(nullptr)
		, publisher(nullptr)
		, subscriber(nullptr)
	{
		eprosima::fastdds::dds::DomainParticipantQos participantQos;
		participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SIMPLE;
		participantQos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = eprosima::fastrtps::Duration_t(1, 2);
		participantQos.name(partName_);

		this->participant = eprosima::fastdds::dds::DomainParticipantFactory::
			get_instance()->create_participant(domain_, participantQos);

		if(this->participant == nullptr){
			throw std::runtime_error("Error: could not create participant");
		}

		this->publisher = new dls::version2::Publisher(this->participant);
		this->subscriber = new dls::version2::Subscriber(this->participant);
	}


	DDSParticipant::~DDSParticipant()
	{
		delete this->publisher;
		delete this->subscriber;
	
		eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->
			delete_participant(this->participant);
	}

	void DDSParticipant::sendMessage(void *msg)
	{
		this->publisher->publish(msg);
	}

	bool DDSParticipant::addWriter(dls::topicType topicData_)
	{
		auto topic = this->addTopic(topicData_);

		if (topic == nullptr)
			return false;

		return this->publisher->addDataWriter(topic);
	}

	eprosima::fastdds::dds::DataReader *DDSParticipant::addReader(
		dls::topicType 					topicData_,
		std::function<void(void *)> 	callback
	){
		auto topic = this->addTopic(topicData_);

		// error could not add topic
		if (topic == nullptr)
			return nullptr;
				
		return this->subscriber->addDataReader(topic, callback);
	}

	eprosima::fastdds::dds::Topic* DDSParticipant::addTopic(dls::topicType topicData_){

		auto search = this->topics.find(topicData_.first);

		if (search != topics.end())
			return search->second;

		this->participant->register_type(topicData_.second);
	
		auto topic = this->participant->create_topic(
			topicData_.first, 
			topicData_.second.get_type_name(),
			eprosima::fastdds::dds::TOPIC_QOS_DEFAULT
		);

		if(topic == nullptr){
			// throw std::runtime_error(
			// 	"Error: could not create publisher topic"
			// );
		}

		this->topics.insert({topicData_.first, topic});

		return topic;
	}


	std::vector<std::string> DDSParticipant::getParticipants(){
		return this->participant->get_participant_names();
	}
	
} // namespace dls
/// \endcond

#endif /* end of include guard: PARTICIPANT_CPP */