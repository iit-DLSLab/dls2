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
	typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;
public:
	// TODO add pointer to robot model
	Controller
	(
		const std::shared_ptr<Dog> &dog_,
		const std::string &name_,
		const period_t &period_
	) :
		pDog(dog_),
		name(name_),
		period(period_)
	{ }

	virtual ~Controller() = default;

	/// Runs the controller. Calls the virtual function run with the correct
	/// paramters at each loop
	AppLayerComponent::Status run() override;

	// This will be replaced by the shutdown and eStop functions
	// /// TODO unknown what this should do
	// virtual void kill() = 0;

protected:
	/// Function gets called each epoch.
	///
	/// @param time The time when this function is called,
	virtual bool run(const std::chrono::system_clock::time_point &time) = 0;

	const std::shared_ptr<Dog> pDog;
	const std::string name;
	const period_t period;
};

#endif /* end of include guard: CONTROLLER_HPP_RSFU8GQS */
