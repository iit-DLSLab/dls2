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
	pDog(dog_),
	name(name_),
	period(period_),
	signal_reconstruction_method(reconst_meth),
	ID(name_),
	pGait_signal(nullptr),
	gait_signal_mutex(),
	pControl_signal(nullptr),
	pControl_signal_mutex(),
	should_run(false)
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
AppLayerComponent::Status Controller::run()
{
	setStatus(Status::RUNNING);
	this->should_run = true;
	do
	{
		// Calculate when the next period needs to start
		auto next_loop_time = this->period + std::chrono::system_clock::now();

		// Run the controller epoch
		run(std::chrono::system_clock::now());

		// Check realtime
		if(std::chrono::system_clock::now() > next_loop_time)
		{
			setStatus(Status::BREAKING_REALTIME);
		}

		// Sleep until the next epoch
		TODO("use realtime sleep here")
		TODO("Check realtime here")
		std::this_thread::sleep_until(next_loop_time);
	}
	while(this->should_run);
	return this->getStatus();
}

AppLayerComponent::Status Controller::stop()
{
	DMSG("controller stop");
	this->should_run = false;
	this->setStatus(Status::STOPPED);
	return this->getStatus();
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
