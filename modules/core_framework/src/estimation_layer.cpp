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
	ddsMonitor(new dls::DDSWriter(
		"EstimatorLayer::monitor",
		dls::domains::estimators,
		dls::topics::command_send
	))
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
		"removeEstimator",
		"Remove a state estimator plugin",
		std::function<bool(std::string)>([&](std::string s)->bool
        {
			if(this->removeEstimator(s))
			{
				if(this->numOfEstimators() == 0)
					return true;
			}

			return false;
		}),
		{{1,0}},
		true
	);

	scout << "ESTIMATION LAYER LOADED" << std::endl;
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
	while(!this->should_quit)
	{
		// TODO("Watchdog over estimators here")
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(300));
	}
	return getStatus();
}

AppLayer::Status EstimationLayer::shutdown()
{
	this->should_quit = true;
	return getStatus();
}

// =============================================================================
// Implementation
// =============================================================================
bool EstimationLayer::loadEstimator(const Estimator::ID_t& lib_name)
{
    std::shared_ptr<EstimatorData> pData = std::make_shared<EstimatorData>();

    for(long unsigned int i = 0; i <= estimators.size(); i++){
        pData->ID = lib_name + "_" + std::to_string(i);
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
			pData->ID,
			lib_name,
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

bool EstimationLayer::removeEstimator(const Estimator::ID_t& ID)
{
	// std::lock_guard<std::mutex> lock(this->estimators_mutex);

	// Find estimator inside the estimators list
	auto res = this->estimators.find(ID);

	if (res == this->estimators.end())
	{
		scout << "Estimator " + ID + " is not loaded" << std::endl;
		return false;
	}

	auto pData = res->second;

    //shutdown controller over the dds comunication layer
	CommandSendMsg msg;
	msg.name(pData->ID);
	msg.command("shutdown");
	this->ddsMonitor->sendMessage((void*) &msg);

    //wait a little for hardware to exit
	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(200));

	if(pData->proc->running()){
		scout << "### ESTIMATOR IS STILL RUNNING WAITING A LITTLE TO GET PROPPER EXIT ###" << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		if(pData->proc->running()){
			scout << "### FORCING ESTIMATOR " << pData->proc->id() << " TO EXIT ###" << std::endl;
			kill(pData->proc->id(), SIGKILL);
		}
	}

	pData->proc = nullptr;
	pData->dds_reader = nullptr;
	this->estimators.erase(pData->ID);

    return true;
}

int EstimationLayer::numOfEstimators()
{
	return this->estimators.size();
}