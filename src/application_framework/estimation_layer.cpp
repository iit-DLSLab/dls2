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
	estimators_mutex(),
	should_run(true)
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
TODO("Unimplemented")
AppLayer::Status EstimationLayer::run()
{
	while(this->should_run)
	{
		DMSG("_____ESTIMATION LAYER_____");


		TODO("proper sleep here")
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(300));
	}
	return getStatus();
}

TODO("Unimplemented")
AppLayer::Status EstimationLayer::shutdown()
{
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
