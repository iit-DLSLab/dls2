/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#include "dls2/core_framework/estimation_layer.hpp"
#include "dls2/class_loader.hpp"

// TODO("temp includes")
#include <chrono>
#include <thread>

using namespace dls;

EstimatorData::EstimatorData() 
    : proc(nullptr)
    , dds_reader(nullptr)
    , ID("")
{ }

EstimatorData::~EstimatorData(){}

EstimationLayer::EstimationLayer(std::string ID) :
	AppLayer(ID),
	estimators(),
	estimators_mutex(),
	should_run(true)
{ 
	command_manager.addCommand<std::string>
	(
		"loadEstimator",
		"Load a state estimator plugin",
		std::function<bool(std::string)>([&](std::string s)->bool
        {
			return this->loadEstimator(s);
		}),
		{{0,1},{2,3}},
		true
	);
}

EstimationLayer::~EstimationLayer()
{
	std::lock_guard<std::mutex> lock(this->estimators_mutex);
}

// =============================================================================
// Interface Override Functions
// =============================================================================
AppLayer::Status EstimationLayer::run()
{
	while(this->should_run)
	{
		// TODO("Watchdog over estimators here")
		// TODO("proper sleep here")
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
bool EstimationLayer::loadEstimator(const std::string &name)
{
    std::shared_ptr<EstimatorData> pData = std::make_shared<EstimatorData>();

    for(long unsigned int i = 0; i <= estimators.size(); i++){
        pData->ID = name + "_" + std::to_string(i);
        if(this->estimators.find(pData->ID) == this->estimators.end())
            break;
    }   

	{
		// std::lock_guard<std::mutex> lock(this->estimators_mutex);

		// launch the hardware
		char *child_process_launcher = std::getenv("DLS_CHILD_PROCESS_LAUNCHER");
		if(!child_process_launcher)
		{
			std::cerr <<
				"ERROR: env variable DLS_CHILD_PROCESS_LAUNCHER not "
				"defined.  This is probably an error with the launch script"
			<< std::endl;
			return false;
		}

		std::cout << "Launching: child_process_launcher" << std::endl;
		pData->proc = std::make_shared<boost::process::child>(std::vector<std::string>({
			child_process_launcher,
			name,
			"estimator",
			"aliengo",
            "live"
		}));

		if (pData->proc == nullptr){
			std::cout << "Estimator process failed to launch" << std::endl;
			return false;
		}

        if (!pData->proc->running()){
			std::cout << "Estimator process failed to launch" << std::endl;
			return false;
		}

		std::cout << "ESTIMATOR " << pData->ID << " IS ON" <<  std::endl;

		this->estimators.emplace(pData->ID, pData);
	}

	return true;
}