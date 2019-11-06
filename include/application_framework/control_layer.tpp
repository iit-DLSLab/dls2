#ifndef CONTROL_LAYER_TPP_COCUF9QG
#define CONTROL_LAYER_TPP_COCUF9QG

// =============================================================================
// Includes
// =============================================================================
#include "application_framework/control_layer.hpp"
#include "util/debug/debug.hpp"

#include <type_traits>
#include <sstream>
#include <stdexcept>

// =============================================================================
// Controlles
// =============================================================================
template <typename controller_t>
void ControlLayer::addController(const std::shared_ptr<controller_t> &pController)
{
	DMSG("ADDING CONTROLLER");
	static_assert
	(
		std::is_base_of<Controller, controller_t>::value,
		"Error: controller_t must inherit from Controller"
	);

	std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
	this->controllers_b.insert
		(
			std::pair
				<
					Controller::ID_t,
					ControllerData
				>
				(
					pController->getID(),
					ControllerData
					{
						.pController = std::static_pointer_cast<Controller>(pController),
						.pExecution_thread = nullptr,
						.pSubscriber = nullptr
					}
				)
		);
}
// =============================================================================
// Gait Generators
// =============================================================================
template <typename generator_t>
void ControlLayer::addGaitGenerator(const std::shared_ptr<generator_t> &pGen)
{
	static_assert
	(
		std::is_base_of<GaitGenerator, generator_t>::value,
		"Error: generator_t must inherit from Gait Generator"
	);

	std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
	this->generators.insert
		(
			std::pair
				<
					GaitGenerator::ID_t,
					std::shared_ptr<GaitGenerator>
				>
				(
					pGen->getID(),
					std::static_pointer_cast<GaitGenerator>(pGen)
				)
		);
}

#endif /* end of include guard: CONTROL_LAYER_TPP_COCUF9QG */
