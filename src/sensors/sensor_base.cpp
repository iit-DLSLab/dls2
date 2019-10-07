#include "sensors/sensor_base.hpp"

// =============================================================================
// Constructors
// =============================================================================
SensorBase::SensorBase(Criticality c) :
	criticality(c),
	status(Status::UNINITIALISED),
	status_mutex()
{ }

// =============================================================================
// Member Functions
// =============================================================================
SensorBase::Criticality SensorBase::getCriticality() const
{
	return this->criticality;
}

SensorBase::Status SensorBase::getStatus() const
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void SensorBase::setStatus(Status s)
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	this->status = s;
}
