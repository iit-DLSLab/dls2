#include "estimator/dummy_estimator.hpp"
#include "util/debug/debug.hpp"

DummyEstimator::DummyEstimator() :
	Estimator("Dummy Estimator", std::chrono::duration<double>(1))
{ }

void DummyEstimator::run(const std::chrono::system_clock::time_point&)
{ }

extern "C" Estimator *create()
{
	auto p = new DummyEstimator;
	return p;
}

extern "C" void destroy(Estimator *p)
{
	delete p;
}
