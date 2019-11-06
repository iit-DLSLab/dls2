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
	PeriodicAppLayerComponent(period_),
	pRobot(pRobot_),
	ID(ID_),
	pData(nullptr),
	data_mutex(),
	publisher("GaitSignal")
{ }

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
	publisher.publish(p);
}

TODO("remove this function")
std::shared_ptr<GaitSignal> GaitGenerator::readSignal()
{
	// std::lock_guard<std::mutex> lock(this->data_mutex);
	// return this->pData;

	DMSG("Warning: using deprecated function");
	return nullptr;
}
