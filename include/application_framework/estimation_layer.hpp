#ifndef ESTIMATION_LAYER_HPP_3QHYDR67
#define ESTIMATION_LAYER_HPP_3QHYDR67

#include <memory>
#include <map>
#include <mutex>
#include <atomic>
#include <string>
#include <thread>

#include "application_framework/app_layer.hpp"
#include "estimator/estimator.hpp"

TODO("Document this class")
class EstimationLayer : public AppLayer
{
public:
	EstimationLayer();
	~EstimationLayer();

	Status run() override;
	Status shutdown() override;

	template <typename estimator_t>
	void addEstimator(const std::shared_ptr<estimator_t>&);

	void loadEstimator(const std::string&);

	TODO("These two should probably return bool")
	void activateEstimator(const Estimator::ID_t&);
	void deactivateEstimator(const Estimator::ID_t);

private:
	// BEGIN critical section
		std::map<Estimator::ID_t, std::shared_ptr<Estimator>> estimators;
		std::map<Estimator::ID_t, std::thread> estimator_threads;
		std::mutex estimators_mutex;
	// END critical section
	std::atomic_bool should_run;
};

#include "application_framework/estimation_layer.tpp"

#endif /* end of include guard: ESTIMATION_LAYER_HPP_3QHYDR67 */
