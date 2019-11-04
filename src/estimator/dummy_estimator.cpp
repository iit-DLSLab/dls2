#include "estimator/dummy_estimator.hpp"
#include "util/debug/debug.hpp"

DummyEstimator::DummyEstimator() :
	Estimator("Dummy Estimator", std::chrono::duration<double>(1))
{ }

void DummyEstimator::run(const std::chrono::system_clock::time_point&)
{
	DMSG("Run dummy estimator");
}

extern "C" Estimator *create()
{
	auto p = new DummyEstimator;
	DMSG("Create dummy estimator");
	return p;
}

extern "C" void destroy(Estimator *p)
{
	DMSG("destroy dummy estimator");
	delete p;
}
