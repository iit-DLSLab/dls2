#include "gait_generator/dummy_gait_generator.hpp"
#include "util/debug/debug.hpp"

DummyGaitGenerator::DummyGaitGenerator(const std::shared_ptr<Dog> &pDog) :
	GaitGenerator
	(
		pDog,
		"dummy_gait_generator",
		std::chrono::duration<double>(1)
	)
{ }

DummyGaitGenerator::DummyGaitGenerator()
	: DummyGaitGenerator(std::make_shared<Dog>())
{ }

void DummyGaitGenerator::run(const std::chrono::system_clock::time_point &time)
{
	auto pData = std::make_shared<GaitSignal>();
	publishData(pData);
	time.time_since_epoch();
}

extern "C" GaitGenerator *create()
{
	auto p = new DummyGaitGenerator;
	// return new DummyController(std::make_shared<Dog>());
	DMSG("I built a dummy gait generator");
	return p;
}

extern "C" void destroy(GaitGenerator *p)
{
	delete p;
	DMSG("I deleted a dummy gait generator");
}
