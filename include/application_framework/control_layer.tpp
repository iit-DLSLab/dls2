#ifndef CONTROL_LAYER_TPP_COCUF9QG
#define CONTROL_LAYER_TPP_COCUF9QG

#include <type_traits>

template <typename controller_t>
void ControlLayer::addController(std::shared_ptr<controller_t> &pController)
{
	static_assert
	(
		std::is_base_of<Controller, controller_t>::value,
		"Error: controller_t must inherit from Controller"
	);

	std::lock_guard<std::mutex> lock(this->components_mutex);
	this->controllers.insert
		(
			std::pair
				<
					Controller::ID_t,
					std::shared_ptr<Controller>
				>
				(
					pController->getID(),
					std::static_pointer_cast<Controller>(pController)
				)
		);
}


#endif /* end of include guard: CONTROL_LAYER_TPP_COCUF9QG */
