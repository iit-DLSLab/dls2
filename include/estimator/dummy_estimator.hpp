#ifndef DUMMY_ESTIMATOR_HPP_7RAUGQJD
#define DUMMY_ESTIMATOR_HPP_7RAUGQJD

#include "estimator/estimator.hpp"

class DummyEstimator : public Estimator
{
public:
	DummyEstimator();
	void run(const std::chrono::system_clock::time_point&) override;

	Status eStop()override{return getStatus();}

private:
};

#endif /* end of include guard: DUMMY_ESTIMATOR_HPP_7RAUGQJD */
