
#include "dls2/core_framework/estimation_layer.hpp"
#include "dls2/util/utils.hpp"
#include "dls2/class_loader.hpp"

// TODO("temp includes")
#include <chrono>
#include <thread>

using namespace dls;

EstimationLayer::EstimationLayer(std::string ID, const std::string& robot_name) :
	Layer(ID, 300),
	estimators(),
	estimators_mutex(),
	robot_name(robot_name)
	// ddsMonitor(std::make_shared<dls::DDSWriter>(
	// 	"EstimatorLayer::monitor",
	// 	dls::domains::estimators,
	// 	dls::topics::command_send
	// ))
{ 
	command_manager.addCommand<std::string>
	(
		"loadEstimator",
		"Load a state estimator plugin",
		std::function<bool(std::string)>([&](std::string s)->bool
        {
			return this->loadEstimator(s);
		}),
		{{0,1},{1,1}},
		true
	);

	command_manager.addCommand<std::string>
	(
		"unloadEstimator",
		"Unload a state estimator plugin",
		std::function<bool(std::string)>([&](std::string s)->bool
        {
			if(this->unloadEstimator(s))
			{
				if(this->numOfEstimators() == 0)
					return true;
			}

			return false;
		}),
		{{1,0}},
		true
	);
}

EstimationLayer::~EstimationLayer()
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================

void EstimationLayer::close()
{
	std::vector<std::string> keys;
	for(auto pair : this->estimators)
		keys.push_back(pair.first);
	
	for(auto key : keys)
		this->unloadEstimator(key);
}

// =============================================================================
// Implementation
// =============================================================================
bool EstimationLayer::loadEstimator(const Estimator::ID_t& lib_name)
{
    std::shared_ptr<AppData> pData;

	if(this->estimators.find(lib_name) == this->estimators.end())
	{
		pData = std::make_shared<AppData>(lib_name);
	}
	else
	{
		this->app_logger.error("estimator " + lib_name + " already loaded");
		return false;
	}
	
	{
		// std::lock_guard<std::mutex> lock(this->estimators_mutex);

		// launch the hardware
		char *child_process_launcher = std::getenv("DLS_CHILD_PROCESS_LAUNCHER");
		if(!child_process_launcher)
		{
			this->app_logger.error(
				"ERROR: env variable DLS_CHILD_PROCESS_LAUNCHER not "
				"defined.  This is probably an error with the launch script"
		);
			return false;
		}

		pData->proc = std::make_shared<boost::process::child>(std::vector<std::string>({
			child_process_launcher,
			pData->getID(),
			lib_name,
			"estimator",
			robot_name
		}));

		if (pData->proc == nullptr){
			this->app_logger.error("Estimator " + lib_name +" failed to launch: nullptr");
			return false;
		}

		this->estimators.emplace(pData->getID(), pData);
	}

	return true;
}

bool EstimationLayer::unloadEstimator(const Estimator::ID_t& ID)
{
	// std::lock_guard<std::mutex> lock(this->estimators_mutex);

	// Find estimator inside the estimators list
	auto res = this->estimators.find(ID);

	if (res == this->estimators.end())
	{
		this->app_logger.error("Estimator " + ID + " is not loaded");
		return false;
	}

	auto pData = res->second;

	command_manager.callCommand("shutdown", {}, pData->getID());
	
	bool unloaded = false;
	if(!utils::wait(std::function<bool()>([&](){
			if(pData->proc->running()){
				return false;
			}
			return true;
		}), 2000, 10, unloaded)){
		std::cout << "### FORCING ESTIMATOR " << ID << " EXIT ###" << std::endl;
		kill(pData->proc->id(), SIGKILL);		
	}

	std::cout << "Estimator " + ID + " is unloaded" << std::endl;

	pData->proc = nullptr;
	this->estimators.erase(pData->getID());

    return true;
}

int EstimationLayer::numOfEstimators()
{
	return this->estimators.size();
}

void EstimationLayer::monitor(){
	this->checkAppData(this->estimators);
}