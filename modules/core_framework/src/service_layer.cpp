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
#include "dls2/core_framework/service_layer.hpp"

#include "dls2/class_loader.hpp"
#include "dls2/core_framework/options.hpp"

using namespace dls;


ServiceLayer::ServiceLayer(std::string ID_) 
	: Layer(ID_)
	, ddsMonitor(std::make_shared<dls::DDSWriter>(
		"ServiceLayer::monitor",
		dls::domains::services,
		dls::topics::command_send
	))
{
    command_manager.addCommand<std::string>
	(
		"loadService",
		"Load a service",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			return this->loadService(type);
        }),
		{{0,1},{1,1}},
		true
	);

	command_manager.addCommand<std::string>
	(
		"unloadService",
		"Remove service",
		std::function<bool(std::string)>([&](std::string s)->bool
        {
			if(this->unloadService(s))
			{
				if(this->numOfServices() == 0)
					return true;
			}

			return false;
		}),
		{{1,0}},
		true
	);
}

ServiceLayer::~ServiceLayer()
{
	scout_sys << "#### SERVICE LAYER OFF ####" << std::endl;
}

AppStatus ServiceLayer::run()
{
	// TODO("Check status of all components in the service layer, take corrective actions if requred")
	setStatus(AppStatus::RUNNING);

	while(!this->shouldQuit())
	{
		// TODO this loop should be used to verify the state of the layer and all its controllers		
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
	}
	return getStatus();
}


bool ServiceLayer::loadService(const std::string& lib_name)
{
	
	if(this->services.find(lib_name) != this->services.end())
	{
		scout_err << "service " + lib_name + " already loaded" << std::endl;
		return false;
	}

    std::shared_ptr<AppData> pData = std::make_shared<AppData>(lib_name);
    
	{
		// std::lock_guard<std::mutex> lock(this->services_mutex);

		// launch the service
		char *child_process_launcher = std::getenv("DLS_CHILD_PROCESS_LAUNCHER");
		if(!child_process_launcher)
		{
			scout_err <<
				"env variable DLS_CHILD_PROCESS_LAUNCHER not "
				"defined.  This is probably an error with the launch script"
			<< std::endl;
			return false;
		}

		pData->proc = std::make_shared<boost::process::child>(std::vector<std::string>({
			child_process_launcher,
			pData->getID(),
			lib_name,
			"service",
			""
		}));

		if (pData->proc == nullptr || pData->proc->wait_for(std::chrono::duration<double, std::milli>(1000))){
			scout_err << "Service " << lib_name << " failed to launch" << std::endl;
			return false;
		}

		scout_sys << "SERVICE " << pData->getID() << " IS ON" <<  std::endl;

		this->services.emplace(pData->getID(), pData);
	}

	return true;
}

bool ServiceLayer::unloadService(const std::string ID)
{
	// std::lock_guard<std::mutex> lock(this->services_mutex);

	// Find service inside the services list
	auto res = this->services.find(ID);

	if (res == this->services.end())
	{
		scout_err << "Service " + ID + " is not loaded" << std::endl;
		return false;
	}

	auto pData = res->second;

    //shutdown service over the dds comunication layer
	command_manager.callCommand("exit", {}, ID);

    // wait a little for service to exit
	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));

	if(pData->proc->running()){
		scout_warn << "### SERVICE " << ID <<" IS STILL RUNNING WAITING A LITTLE TO GET PROPPER EXIT ###" << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		if(pData->proc->running()){
			scout_warn << "### FORCING SERVICE " << ID <<" TO EXIT ###" << std::endl;
			kill(pData->proc->id(), SIGKILL);
		}
	}
	else
	{
		scout_sys << "Service " + ID + " is unloaded" << std::endl;
	}

	pData->proc = nullptr;
	this->services.erase(ID);
    return true;
}

AppStatus ServiceLayer::stop()
{
	std::vector<std::string> keys;
	for(auto pair : this->services)
		keys.push_back(pair.first);
	
	for(auto key : keys)
		this->unloadService(key);

	this->should_quit = true;

	setStatus(AppStatus::STOPPED);
	return getStatus();
}

int ServiceLayer::numOfServices()
{
	return this->services.size();
}