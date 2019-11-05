#include "gait_generator/gait_generator.hpp"
#include <chrono>
#include <thread>
#include "util/debug/debug.hpp"

// fastrtps
#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/Domain.h>

// =============================================================================
// Constructors
// =============================================================================
GaitGenerator::GaitGenerator
(
	const std::shared_ptr<Dog> &pRobot_,
	const ID_t &ID_,
	const period_t &period_
) :
	PeriodicAppLayerComponent(period_),
	pRobot(pRobot_),
	ID(ID_),
	pData(nullptr),
	data_mutex(),
	pParticipant(nullptr),
	pPublisher(nullptr),
	listener(),
	rtps_type()
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
	pub_attr.topic.topicName = "HelloWorldPubSubTopic";

	TODO("not cleaning the publisher because it's generating a library error")
	pPublisher.reset
	(
		eprosima::fastrtps::Domain::createPublisher
		(
			pParticipant.get(),
			pub_attr,
			static_cast<PubListener*>(&listener)
		),
		[](eprosima::fastrtps::Publisher*){}
	);
	TODO("Check nullptr above")
}

// =============================================================================
// Implementation
// =============================================================================
GaitGenerator::ID_t GaitGenerator::getID()
{
	return this->ID;
}
void GaitGenerator::publishData(const GaitSignal &signal)
{
	GaitSignalMsg p = signal;
	pPublisher->write(&p);
}

// TODO("Make one that doesn't take pointer")
// [[deprecated]]
// void GaitGenerator::publishData(const std::shared_ptr<GaitSignal> &pIn_data)
// {
// 	DMSG("CALLING DEPRECATED FUNCTION");
// 	// {
// 	// 	std::lock_guard<std::mutex> lock(this->data_mutex);
// 	// 	this->pData = pIn_data;
// 	// }

// 	GaitSignalMsg p = *pIn_data;
// 	// pPublisher->write(&p);
// }

std::shared_ptr<GaitSignal> GaitGenerator::readSignal()
{
	std::lock_guard<std::mutex> lock(this->data_mutex);
	return this->pData;
}

// =============================================================================
// FastRTPS
// =============================================================================
// any overrides to the publisher listener can be put here
