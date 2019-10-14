#ifndef CONTROL_LAYER_HPP_YCHFNYBM
#define CONTROL_LAYER_HPP_YCHFNYBM

#include "application_framework/app_layer.hpp"
#include "controller/controller.hpp"

#include <map>
#include <memory>
#include <mutex>

class ControlLayer : public AppLayer
{
public:
	ControlLayer();

	// TODO these are not implemented
	Status run() override;
	Status shutdown() override;

	/// Adds a controller to the control layer
	///
	/// This call does not start the controller. see
	/// ControlLayer::activateController
	template <typename controller_t>
	void addController(std::shared_ptr<controller_t>&);

	/// Activates a controller
	///
	/// @ret true if the controller exists, false otherwise.
	/// See also ControlLayer::deactivateController
	bool activateController(Controller::ID_t);

	/// Deactivates a controller
	///
	/// @ret true if the controller exists, false otherwise
	/// See also ControlLayer::activateController
	bool deactivateController(Controller::ID_t);

private:
	// BEGIN critical section
		std::map<Controller::ID_t, std::shared_ptr<Controller>> controllers;
		std::mutex controllers_mutex;
	// END critical section
};

#include "application_framework/control_layer.tpp"

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
