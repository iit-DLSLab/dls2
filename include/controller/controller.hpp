#ifndef CONTROLLER_HPP_RSFU8GQS
#define CONTROLLER_HPP_RSFU8GQS

// #include <chrono>
#include <string>
#include <memory>

#include "application_framework/components/app_layer_component.hpp"

// TODO temp, remove and insert Dog class in correct place
class Dog {};

// TODO a lot of functions have been removed from Controller. Many of them need
// to be put into Dog

class Controller : public AppLayerComponent
{
protected:
	typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;
public:

	Controller
	(
		const std::shared_ptr<Dog> &dog,	///< A pointer to the robot model
		const std::string &name,			///< The name of the controller
		const period_t &period				///< The period of the controller
	);

	virtual ~Controller() = default;

	/// Runs the controller. Calls the virtual function run with the correct
	/// paramters at each loop
	/// Function should set state
	AppLayerComponent::Status run() override;

	// This will be replaced by the shutdown and eStop functions
	// /// TODO unknown what this should do
	// virtual void kill() = 0;

protected:
	/// Function gets called each epoch.
	///
	/// @param time The time when this function is called,
	virtual void run(const std::chrono::system_clock::time_point &time) = 0;

	const std::shared_ptr<Dog> pDog;
	const std::string name;
	const period_t period;
};

#endif /* end of include guard: CONTROLLER_HPP_RSFU8GQS */
