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
	DMSG("Dummy Controller-+-");
	// static int cnt = 0;
	// DMSG("Running " << name << " ");
	auto pGait_signal = this->readGaitSignal();
	if(!pGait_signal) DMSG("No gait signal published");
	else DLOG(pGait_signal->desired_com_pose.toPosition().transpose());

	time.time_since_epoch();
}

extern "C" Controller *create()
{
	auto p = new DummyController;
	DMSG("I built a dummy controller");
	return p;
}

extern "C" void destroy(Controller *p)
{
	delete p;
	DMSG("I deleted a dummy controller");
}
