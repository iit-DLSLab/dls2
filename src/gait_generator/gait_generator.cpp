#include "gait_generator/gait_generator.hpp"

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
	period(period_)
{ }

// =============================================================================
// Implementation
// =============================================================================
GaitGenerator::ID_t GaitGenerator::getID()
{
	return this->ID;
}
