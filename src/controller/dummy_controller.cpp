#include "controller/dummy_controller.hpp"
DummyController::DummyController
(
	const std::shared_ptr<Dog> &dog
) :
	Controller
	(
		dog,
		"dummy_controller",
		std::chrono::duration<double>(1),
		SignalReconstructionMethod::ZERO_ORDER_HOLD
	)
{}

DummyController::DummyController() :
	DummyController(std::make_shared<Dog>())
{ }

void DummyController::run(const std::chrono::system_clock::time_point &time)
{
	static int cnt = 0;
	std::cout << "Running " << name << " " << std::endl;
	time.time_since_epoch();
	if(++cnt == 5)
	{
		setStatus(Status::SUCCESS);
	}
}

extern "C" Controller *create()
{
	auto p = new DummyController;
	// return new DummyController(std::make_shared<Dog>());
	std::cout << "I built a dummy controller" << std::endl;
	return p;
}

extern "C" void destroy(Controller *p)
{
	delete p;
	std::cout << "I deleted a dummy controller" << std::endl;
}
