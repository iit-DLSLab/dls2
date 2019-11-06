#include "controller/dummy_controller.hpp"
#include "util/debug/debug.hpp"
DummyController::DummyController
(
	const std::shared_ptr<Dog> &dog
) :
	Controller
	(
		dog,
		"dummy_controller",
		std::chrono::duration<double>(1),
		ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
	)
{ }

DummyController::DummyController() :
	DummyController(std::make_shared<Dog>())
{ }

void DummyController::run(const std::chrono::system_clock::time_point &time)
{
	auto pGait_signal = this->readGaitSignal();

	ControlSignal s;
	publishSignal(s);

	time.time_since_epoch();
}

extern "C" Controller *create()
{
	auto p = new DummyController;
	return p;
}

extern "C" void destroy(Controller *p)
{
	delete p;
}
