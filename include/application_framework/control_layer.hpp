#ifndef CONTROL_LAYER_HPP_YCHFNYBM
#define CONTROL_LAYER_HPP_YCHFNYBM

#include "application_framework/app_layer.hpp"
#include "controller/controller.hpp"
#include "gait_generator/gait_generator.hpp"

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

	// ============================== Controllers ==============================
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
	bool activateController(const Controller::ID_t&);

	/// Deactivates a controller
	///
	/// @ret true if the controller exists, false otherwise
	/// See also ControlLayer::activateController
	bool deactivateController(const Controller::ID_t&);

	/// Dynamically loads a controller at run time
	///
	/// This function throws a std::runtime_error if the controller shared
	/// object cannot be found
	void loadController(const std::string &name);

	// ============================ Gait Generators ============================
	// template <typename generator_t>
	// void addGaitGenerator(std::shared_ptr<generator_t>&);
	// bool activateGaitGenerator(GaitGenerator::ID_t);
	// void deactivateGaitGenerators();
	// void loadGaitGenerator(const std::string &name);

private:
	template <class T>
	static std::shared_ptr<T> loadClass(const std::string &name);

	// BEGIN critical section
		std::map<Controller::ID_t, std::shared_ptr<Controller>> controllers;
		std::mutex controllers_mutex;
	// END critical section
	// BEGIN critical section
		// std::map<GaitGenerator::ID_t, std::shared_ptr<GaitGenerator>> generators;
		// std::mutex gait_generators_mutex;
	// END critical section
};

#include "application_framework/control_layer.tpp"

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
