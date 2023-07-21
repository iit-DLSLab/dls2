
#ifndef DUMMY_ESTIMATOR_HPP_7RAUGQJD
#define DUMMY_ESTIMATOR_HPP_7RAUGQJD

#include "dls2/estimator/estimator.hpp"

namespace dls
{
class DummyEstimator : public Estimator
{
public:
	DummyEstimator();
	void run(const std::chrono::system_clock::time_point&) override;

	AppStatus eStop()override{return getStatus();}
	std::string where() override;

private:
};
} // end namespace dls
#endif /* end of include guard: DUMMY_ESTIMATOR_HPP_7RAUGQJD */
