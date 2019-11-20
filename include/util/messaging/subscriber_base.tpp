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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef SUBSCRIBER_BASE_TPP_YC8QGBV2
#define SUBSCRIBER_BASE_TPP_YC8QGBV2

#include "util/messaging/subscriber_base.hpp"

#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/TopicDataType.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>

template <class PubSub_t>
PubSub_t SubscriberBase<PubSub_t>::rtps_type;

template <class PubSub_t>
SubscriberBase<PubSub_t>::SubscriberBase(const std::string &topic) :
	pParticipant(nullptr),
	pSubscriber(nullptr)
{
	eprosima::fastrtps::ParticipantAttributes participant_attr;
	participant_attr.rtps.setName("Participant_subscriber");

	auto custom_transport = std::make_shared<eprosima::fastrtps::rtps::UDPv4TransportDescriptor>();
	custom_transport->interfaceWhiteList.emplace_back("127.0.0.1");
	participant_attr.rtps.useBuiltinTransports = false;
	participant_attr.rtps.userTransports.push_back(custom_transport);
	
	TODO("figure out how to remove participant properly")
	pParticipant.reset
	(
		eprosima::fastrtps::Domain::createParticipant(participant_attr),
		[](eprosima::fastrtps::Participant*){}
	);
	eprosima::fastrtps::Domain::registerType
	(
		pParticipant.get(),
		static_cast<eprosima::fastrtps::TopicDataType*>(&rtps_type)
	);

	eprosima::fastrtps::SubscriberAttributes sub_attr;
	sub_attr.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
	sub_attr.topic.topicDataType = rtps_type.getName();

	sub_attr.topic.topicName = topic;

	TODO("figure out how to remove this subscriber")
	pSubscriber.reset
	(
		eprosima::fastrtps::Domain::createSubscriber
		(
			pParticipant.get(),
			sub_attr,
			static_cast<eprosima::fastrtps::SubscriberListener*>(this)
		),
		[](eprosima::fastrtps::Subscriber*){}
	);
}

#endif /* end of include guard: SUBSCRIBER_BASE_TPP_YC8QGBV2 */
