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
		const std::string &partName_,
		const unsigned int &domain_
	)
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


	DDSParticipant::~DDSParticipant(){
		delete this->publisher;
		delete this->subscriber;
	
		eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->
			delete_participant(this->participant);

		delete this->participant;
	}

	bool DDSParticipant::addWriter(
		std::pair<std::string, std::string> topic_
	){

		if(this->topic == nullptr)
			if(!this->addTopic(topic_.first, topic_.second))
				return false;

		return this->publisher->addDataWriter(this->topic);
	}

	bool DDSParticipant::addReader(
		std::string 								topicName_,
		std::string 								dataType_,
		dls::version2::Subscriber::CallbackType 	callback
	){

		if(this->topic == nullptr)
			if(!this->addTopic(topicName_, dataType_))
				return false;

		return this->subscriber->addDataReader(this->topic, callback);
	}

	bool DDSParticipant::addTopic(
		std::string 	topicName_,
		std::string 	dataType_
	){
		
		if(this->topic == nullptr){
			this->topic = this->participant->create_topic(
				topicName_, 
				dataType_,
				eprosima::fastdds::dds::TOPIC_QOS_DEFAULT
			);

			if(this->topic == nullptr){
				// throw std::runtime_error(
				// 	"Error: could not create publisher topic"
				// );
				return false;
			}
		}

		return true;
	}




	std::vector<std::string> DDSParticipant::getParticipants(){
		return this->participant->get_participant_names();
	}
	
} // namespace dls
/// \endcond

#endif /* end of include guard: PARTICIPANT_CPP */