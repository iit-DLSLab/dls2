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

		// this->type.register_type(this->participant);
	}


	DDSParticipant::~DDSParticipant(){
	
		eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->
			delete_participant(this->participant);
	}


	template <class PubSub_t>
	bool DDSParticipant::addSubscriber(){
		subscribers.emplace_back(
			new dls::version2::Subscriber<PubSub_t>(this->participant)
		);

		return true;
	}
} // namespace dls
/// \endcond
