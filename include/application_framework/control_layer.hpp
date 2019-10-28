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

	// ========================== Interface Overrides ==========================
	TODO("These functions are not implemented")
	Status run() override;
	Status shutdown() override;

	// ============================== Controllers ==============================
	/// Adds a controller to the control layer
	///
	/// This call does not start the controller. see
	/// ControlLayer::activateController
	template <typename controller_t>
	void addController(const std::shared_ptr<controller_t>&);

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
	/// Adds a gait generator to the control layer
	///
	/// This call does not start the gait generator. See
	/// ControlLayer::activateGaitGenerator
	template <typename generator_t>
	void addGaitGenerator(const std::shared_ptr<generator_t>&);

	/// Activates a gait generator
	///
	/// This will stop any other running gait generators
	/// @ret true if the controller exists, false otherwise. See also
	/// ControlLayer::deactivateGaitGenerators
	bool activateGaitGenerator(const GaitGenerator::ID_t&);

	/// Deactivates the current gait generator
	void deactivateGaitGenerators();

	/// Dynamically loads a gait generator at run time
	///
	/// This function throws a std::runtime_error if the gait generator shared
	/// object cannot be found
	void loadGaitGenerator(const std::string &name);

private:
	// ================================ Utility ================================
	/// Dynamically loads a class at run time
	///
	/// This function throws a std::runtime_error if the shared object cannot be
	/// found
	template <class T>
	static std::shared_ptr<T> loadClass(const std::string &name);

	TODO("This should be put in the robot class")
	/// Saturates torques so that they do not exceed safe limits
	///
	/// @param req The requested torques
	/// @ret A saturated version of the torques that do not exceed safe limits
	Eigen::MatrixXd saturateTorques(const Eigen::MatrixXd &req) const;

	// ============================ Communincation =============================
	void publishDesiredTorques(const Eigen::MatrixXd &) const;


	// ============================= Data Members ==============================
	// BEGIN critical section
		std::map<Controller::ID_t, std::shared_ptr<Controller>> controllers;
		std::mutex controllers_mutex;
	// END critical section
	// BEGIN critical section
		std::map<GaitGenerator::ID_t, std::shared_ptr<GaitGenerator>> generators;
		std::mutex gait_generators_mutex;
		// ALWAYS check if this is nullptr
		std::shared_ptr<GaitGenerator> currentActiveGenerator;
	// END critical section
};

#include "application_framework/control_layer.tpp"

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
