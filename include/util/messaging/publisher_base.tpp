#ifndef PUBLISHER_BASE_TPP_I5UWXWN8
#define PUBLISHER_BASE_TPP_I5UWXWN8

#include "util/messaging/publisher_base.hpp"

#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/TopicDataType.h>

template <class PubSub_t>
PubSub_t PublisherBase<PubSub_t>::rtps_type;

template <class PubSub_t>
PublisherBase<PubSub_t>::PublisherBase(const std::string &topic) :
	pParticipant(nullptr),
	pPublisher(nullptr)
{
	// Create participant
	eprosima::fastrtps::ParticipantAttributes participant_attr;
	participant_attr.rtps.setName("Participant_publisher");
	TODO("not cleaning the participant because it's generating a library error")
	pParticipant.reset
	(
		eprosima::fastrtps::Domain::createParticipant(participant_attr),
		[](eprosima::fastrtps::Participant*){}
	);
	TODO("Check for null pointer above")

	// register
	eprosima::fastrtps::Domain::registerType
	(
		pParticipant.get(), static_cast<eprosima::fastrtps::TopicDataType*>(&rtps_type)
	);

	// Create publisher
	eprosima::fastrtps::PublisherAttributes pub_attr;
	pub_attr.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
	pub_attr.topic.topicDataType = rtps_type.getName();
	TODO("Change the name here");
	pub_attr.topic.topicName = topic;

	TODO("not cleaning the publisher because it's generating a library error")
	pPublisher.reset
	(
		eprosima::fastrtps::Domain::createPublisher
		(
			pParticipant.get(),
			pub_attr,
			this
		),
		[](eprosima::fastrtps::Publisher*){}
	);
	TODO("Check nullptr above")
}

template<class PubSub_t>
void PublisherBase<PubSub_t>::publish(typename PubSub_t::type &msg)
{
	pPublisher->write(&msg);
}

#endif /* end of include guard: PUBLISHER_BASE_TPP_I5UWXWN8 */
