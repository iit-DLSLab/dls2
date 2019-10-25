#ifndef CONTROLLER_HPP_RSFU8GQS
#define CONTROLLER_HPP_RSFU8GQS

// #include <chrono>
#include <string>
#include <memory>

#include "application_framework/components/app_layer_component.hpp"

TODO("temp, remove and insert Dog class in correct place")
class Dog {};

TODO("a lot of functions have been removed from Controller. Many of them need to be put into Dog")

class Controller : public AppLayerComponent
{
protected:
	TODO("this is repeated in Gait Generator. Refactor this into one location")
	typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;
public:
	using ID_t = std::string;

	// Plugin typedefs
	typedef Controller *create_t();
	typedef void destroy_t(Controller*);

	/// How the control layer should interpret the torques from the controller
	/// when summing the torques between multiple controllers
	TODO("Implement IMPULSE")
	enum class SignalReconstructionMethod
	{
		ZERO_ORDER_HOLD,
		// IMPULSE
	};

	Controller
	(
		const std::shared_ptr<Dog>&,		///< A pointer to the robot model
		const ID_t&,						///< The ID of the controller
		const period_t&,					///< The period of the controller
		const SignalReconstructionMethod&	///< Signal reconstruction used by this controller
	);

	virtual ~Controller() = default;

	/// Runs the controller. Calls the virtual function run with the correct
	/// paramters at each loop
	/// Function should set state
	AppLayerComponent::Status run() override;

	// This will be replaced by the shutdown and eStop functions
	TODO("unknown what this should do")
	// virtual void kill() = 0;

	/// Get the ID of this controller
	///
	/// @ret the ID
	ID_t getID() const;

protected:
	/// Function gets called each epoch.
	///
	/// @param time The time when this function is called,
	virtual void run(const std::chrono::system_clock::time_point &time) = 0;

	const std::shared_ptr<const Dog> pDog;
	const std::string name;
	const period_t period;
	const SignalReconstructionMethod signal_reconstruction_method;
	const ID_t ID;

private:
};

#endif /* end of include guard: CONTROLLER_HPP_RSFU8GQS */
