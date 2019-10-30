#ifndef ESTIMATION_LAYER_HPP_3QHYDR67
#define ESTIMATION_LAYER_HPP_3QHYDR67

#include <memory>
#include <vector>
#include <mutex>
#include <atomic>
#include <string>

#include "application_framework/app_layer.hpp"
#include "estimator/estimator.hpp"

TODO("Document this class")
class EstimationLayer : public AppLayer
{
public:
	EstimationLayer();
	Status run() override;
	Status shutdown() override;

	template <typename estimator_t>
	void addEstimator(const std::shared_ptr<estimator_t>&);

	void loadEstimator(const std::string&);

	void activateEstimator(const Estimator::ID_t&);

	void deactivateEstimator(const Estimator::ID_t);

private:
	// BEGIN critical section
		std::vector<std::shared_ptr<Estimator>> estimators;
		std::mutex estimators_mutex;
	// END critical section
	std::atomic_bool should_run;
};

#include "application_framework/estimation_layer.tpp"

#endif /* end of include guard: ESTIMATION_LAYER_HPP_3QHYDR67 */
