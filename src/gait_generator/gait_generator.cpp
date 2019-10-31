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
	data_mutex()
{ }

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

