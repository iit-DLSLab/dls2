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
		DMSG("_____ESTIMATION LAYER_____");

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
