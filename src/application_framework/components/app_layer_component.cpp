#include "application_framework/components/app_layer_component.hpp"

AppLayerComponent::Status AppLayerComponent::getStatus()
{
	std::lock_guard<std::mutex> lock(this->status);
	return this->status;
}
