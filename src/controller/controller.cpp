#include <thread>
#include "controller/controller.hpp"
#include "todo.h"
#include "util/debug/debug.hpp"

// fastrtps
#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/Domain.h>

// =============================================================================
// Constructors
// =============================================================================
Controller::Controller
(
	const std::shared_ptr<Dog> &dog_,
	const std::string &name_,
	const period_t &period_,
	const ControlSignal::SignalReconstructionMethod &reconst_meth
):
	PeriodicAppLayerComponent(period_),
	pDog(dog_),
	name(name_),
	signal_reconstruction_method(reconst_meth),
	ID(name_),
	pGait_signal(nullptr),
	gait_signal_mutex(),
	pControl_signal(nullptr),
	pControl_signal_mutex(),
	should_run(false),
	pParticipant(nullptr),
	pSubscriber(nullptr),
	listener
	(
		std::shared_ptr<Controller>
		(
			this,
			[](Controller*){} // do not use the shared pointer to delete the object
		)
	),
	rtps_type()
{
	// configure fastrtps subscription
	eprosima::fastrtps::ParticipantAttributes participant_attr;
	participant_attr.rtps.setName("Participant_subscriber");
	TODO("not cleaning the participant because it's generating a library error")
	pParticipant.reset
	(
		eprosima::fastrtps::Domain::createParticipant(participant_attr),
		[](eprosima::fastrtps::Participant*){}
	);

	TODO("Check for nullptr on failure of above")

	// Register
	eprosima::fastrtps::Domain::registerType
	(
		pParticipant.get(),
		static_cast<eprosima::fastrtps::TopicDataType*>(&rtps_type)
	);

	// Create Subscriber
	eprosima::fastrtps::SubscriberAttributes sub_attr;
	sub_attr.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
	sub_attr.topic.topicDataType = rtps_type.getName();
	sub_attr.topic.topicName = "HelloWorldPubSubTopic";
	TODO("not cleaning the subscriber because it's generating a library error")
	pSubscriber.reset
	(
		eprosima::fastrtps::Domain::createSubscriber
		(
			pParticipant.get(),
			sub_attr,
			static_cast<eprosima::fastrtps::SubscriberListener*>(&listener)
		),
		[](eprosima::fastrtps::Subscriber*){}
	);
}

// =============================================================================
// Implementation
// =============================================================================
Controller::ID_t Controller::getID() const
{
	return this->ID;
}

void Controller::pushSignal(const std::shared_ptr<GaitSignal> &pSignal)
{
	std::lock_guard<std::mutex> lock(this->gait_signal_mutex);
	pGait_signal = pSignal;
}

std::shared_ptr<const GaitSignal> Controller::readGaitSignal() const
{
	std::lock_guard<std::mutex> lock(this->gait_signal_mutex);
	return this->pGait_signal;
}

void Controller::publishSignal(const std::shared_ptr<const ControlSignal> &p)
{
	std::lock_guard<std::mutex> lock(this->pControl_signal_mutex);
	this->pControl_signal = p;
}

const std::shared_ptr<const ControlSignal> Controller::readSignal() const
{
	std::lock_guard<std::mutex> lock(this->pControl_signal_mutex);
	return this->pControl_signal;
}

// =============================================================================
// FastRTPS
// =============================================================================
Controller::SubListener::SubListener(std::shared_ptr<Controller> p) :
	pOwner(p),
	info()
{ }

void Controller::SubListener::onNewDataMessage(eprosima::fastrtps::Subscriber *pSub)
{
	DMSG("================GOT A MESSAGE===============");
	GaitSignalMsg st;
	// std::shared_ptr<HelloWorld> st = std::make_shared<HelloWorld>();
	if(pSub->takeNextData(&st, &info))
	{
		// DLOG(st.desired_com_pose()).toPosition().transpose());
		std::lock_guard<std::mutex> lock(pOwner->gait_signal_mutex);
		pOwner->pGait_signal = std::make_shared<const GaitSignal>(st);
		DLOG(pOwner->pGait_signal->desired_com_pose.toPosition().transpose());
	}
}
