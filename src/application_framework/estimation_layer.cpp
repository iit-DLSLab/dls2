#include "application_framework/estimation_layer.hpp"
#include "util/class_loader.hpp"
#include "util/debug/debug.hpp"

TODO("temp includes")
#include <chrono>
#include <thread>

// =============================================================================
// Constructors
// =============================================================================
EstimationLayer::EstimationLayer() :
	estimators(),
	estimator_threads(),
	estimators_mutex(),
	should_run(true)
{ }

EstimationLayer::~EstimationLayer()
{
	std::lock_guard<std::mutex> lock(this->estimators_mutex);

	for(auto &pair_ID_pEstimator : this->estimators)
	{
		pair_ID_pEstimator.second->stop();
	}

	for(auto &pair_ID_thread : this->estimator_threads)
	{
		pair_ID_thread.second.join();
	}
}

// =============================================================================
// Interface Override Functions
// =============================================================================
AppLayer::Status EstimationLayer::run()
{
	while(this->should_run)
	{
		TODO("Watchdog over estimators here")
		TODO("proper sleep here")
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(300));
	}
	return getStatus();
}

AppLayer::Status EstimationLayer::shutdown()
{
	this->should_run = false;
	return getStatus();
}

// =============================================================================
// Implementation
// =============================================================================
void EstimationLayer::loadEstimator(const std::string &name)
{
	std::shared_ptr<Estimator> pEstimator = ClassLoader::loadClass<Estimator>(name);
	this->addEstimator(pEstimator);
}

TODO("This is copied more or less in all the layers")
bool EstimationLayer::activateEstimator(const Estimator::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->estimators_mutex);

	auto estimator_it = this->estimators.find(ID);

	if(estimator_it == this->estimators.end()) return false;

	TODO("check whether estimator is already active or not")

	AppLayerComponent::Status (Estimator::*run_p)() = &Estimator::run;
	this->estimator_threads.emplace
	(
		std::piecewise_construct,
		std::forward_as_tuple(ID),
		std::forward_as_tuple(run_p, &*estimator_it->second)
	);

	return true;
}
