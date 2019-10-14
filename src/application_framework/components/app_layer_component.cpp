#include "application_framework/components/app_layer_component.hpp"

// =============================================================================
// Constructors
// =============================================================================
AppLayerComponent::AppLayerComponent() :
	status(Status::UNCONSTRUCTED),
	status_mutex()
{ }

// =============================================================================
// Class Implementation
// =============================================================================
AppLayerComponent::Status AppLayerComponent::getStatus()
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void AppLayerComponent::setStatus(Status s)
{
	this->status = s;
}
