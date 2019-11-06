#include <thread>
#include "controller/controller.hpp"
#include "todo.h"
#include "util/debug/debug.hpp"
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
	listener
	(
		std::shared_ptr<Controller>
		(
			this,
			[](Controller*){} // do not use the shared pointer to delete the object
		)
	)
{ }

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
	SubscriberBase<GaitSignalMsgPubSubType>("GaitSignal"),
	pOwner(p),
	info()
{ }

void Controller::SubListener::onNewDataMessage(eprosima::fastrtps::Subscriber *pSub)
{
	DMSG("================GOT A MESSAGE===============");
	GaitSignalMsg st;
	if(pSub->takeNextData(&st, &info))
	{
		std::lock_guard<std::mutex> lock(pOwner->gait_signal_mutex);
		pOwner->pGait_signal = std::make_shared<const GaitSignal>(st);
		DLOG(pOwner->pGait_signal->desired_com_pose.toPosition().transpose());
	}
}
