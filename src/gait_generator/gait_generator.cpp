#include "gait_generator/gait_generator.hpp"
#include <chrono>
#include <thread>
#include "util/debug/debug.hpp"

// =============================================================================
// Constructors
// =============================================================================
GaitGenerator::GaitGenerator
(
	const std::shared_ptr<Dog> &pRobot_,
	const ID_t &ID_,
	const period_t &period_
) :
	pRobot(pRobot_),
	ID(ID_),
	period(period_),
	pData(nullptr),
	data_mutex(),
	should_run(false)
{ }

// =============================================================================
// Interface Overrides
// =============================================================================
TODO("move this into PeriodicAppLayerComponent")
TODO("this is mostly a copy-paste of the same function in controller. Refactor")
AppLayerComponent::Status GaitGenerator::run()
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

TODO("this is a copy from controller. Refactor")
AppLayerComponent::Status GaitGenerator::stop()
{
	DMSG("gait generator stop");
	this->should_run = false;
	this->setStatus(Status::STOPPED);
	return this->getStatus();
}

// =============================================================================
// Implementation
// =============================================================================
GaitGenerator::ID_t GaitGenerator::getID()
{
	return this->ID;
}

void GaitGenerator::publishData(const std::shared_ptr<GaitSignal> &pIn_data)
{
	std::lock_guard<std::mutex> lock(this->data_mutex);
	this->pData = pIn_data;
}

std::shared_ptr<GaitSignal> GaitGenerator::readSignal()
{
	std::lock_guard<std::mutex> lock(this->data_mutex);
	return this->pData;
}

