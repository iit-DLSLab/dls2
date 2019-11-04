#include "estimator/estimator.hpp"

// =============================================================================
// Constructors
// =============================================================================
Estimator::Estimator(const ID_t &inID, const period_t &period) :
	PeriodicAppLayerComponent(period),
	ID(inID)
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
// AppLayerComponent::Status Estimator::run()
// {
// 	TODO("Unimplemented")
// 	return getStatus();
// }

// AppLayerComponent::Status Estimator::eStop()
// {
// 	TODO("Unimplemented")
// 	return getStatus();
// }

// AppLayerComponent::Status Estimator::stop()
// {
// 	TODO("Unimplemented")
// 	return getStatus();
// }

// =============================================================================
// Implementation
// =============================================================================
Estimator::ID_t Estimator::getID() const
{
	return this->ID;
}
