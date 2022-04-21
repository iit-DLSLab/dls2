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
#ifndef DDS_READER_TPP
#define DDS_READER_TPP

// =============================================================================
// Old Includes -- To be removed
// =============================================================================
#include "dls2/util/messaging/subscriber.hpp"

#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/participant/Participant.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/TopicDataType.h>
#include <fastrtps/Domain.h>

#include <stdexcept>
#include <sstream>

// =============================================================================
// Old Version - To be Removed
// =============================================================================
namespace dls
{
	template <class PubSub_t>
	DDSReader<PubSub_t>::DDSReader(
		std::string     partName_,
		dls::domainType domain_,
		dls::topicType  topic_,
		std::function<void(void *)> callback_

	)
		: DDSParticipant(partName_, domain_)
	{
		this->addReader(PubSub_t, callback_);
	}

	template <class PubSub_t>
	DDSReader<PubSub_t>::~DDSReader()
	{}
} // end namespace dls

#endif /* end of include guard: DDS_READER_TPP */
