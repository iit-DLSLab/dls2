#include <thread>
#include "controller/controller.hpp"

Controller::Controller
(
	const std::shared_ptr<Dog> &dog_,
	const std::string &name_,
	const period_t &period_,
	const SignalReconstructionMethod &reconst_meth
):
	pDog(dog_),
	name(name_),
	period(period_),
	signal_reconstruction_method(reconst_meth),
	ID(name_)
{ }

AppLayerComponent::Status Controller::run()
{
	decltype(getStatus()) status;
	setStatus(Status::RUNNING);
	do
	{
		// Calculate when the next period needs to start
		auto next_loop_time = this->period + std::chrono::system_clock::now();

		// Run the controller epoch
		run(std::chrono::system_clock::now());

		// Check realtime
		if(std::chrono::system_clock::now() > next_loop_time)
		{
			setStatus(Status::BREAKING_REALTIME);
		}

		// Sleep until the next epoch
		// TODO use realtime sleep here
		std::this_thread::sleep_until(next_loop_time);
	}
	while((status = getStatus()) == Status::RUNNING);
	return status;
}

Controller::ID_t Controller::getID() const
{
	return this->ID;
}
