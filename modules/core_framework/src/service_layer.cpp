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
	: AppLayer(ID_)
{
    ddsMonitor = new dls::DDSWriter(
		"ServiceLayer::monitor",
		dls::domains::services,
		dls::topics::command_send
	);

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
		"removeService",
		"Remove service",
		std::function<bool(std::string)>([&](std::string s)->bool
        {
			if(this->removeService(s))
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
	std::cout << "#### SERVICE LAYER OFF ####" << std::endl;
}

ServiceLayer::Status ServiceLayer::run()
{
	// TODO("Check status of all components in the service layer, take corrective actions if requred")
	setStatus(Status::RUNNING);

	while(!this->shouldQuit())
	{
		// TODO this loop should be used to verify the state of the layer and all its controllers		
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
	}
	return getStatus();
}


bool ServiceLayer::loadService(const std::string& lib_name)
{
	
	if(this->services.find(lib_name) != this->services.end())\
	{
		scout << "SERVICE " + lib_name + " IS ALREADY RUNNING" << std::endl;
		return false;
	}

    std::shared_ptr<AppData> pData = std::make_shared<AppData>();
	pData->ID = lib_name;
    
	{
		// std::lock_guard<std::mutex> lock(this->services_mutex);

		// launch the service
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
			"service",
			"",
            "live"
		}));

		if (pData->proc == nullptr){
			std::cout << "Service process failed to launch" << std::endl;
			return false;
		}

        if (!pData->proc->running()){
			std::cout << "Service process failed to launch" << std::endl;
			return false;
		}

		std::cout << "SERVICE " << pData->ID << " IS ON" <<  std::endl;

		this->services.emplace(pData->ID, pData);
	}

	return true;
}

bool ServiceLayer::removeService(const std::string& ID)
{
	// std::lock_guard<std::mutex> lock(this->services_mutex);

	// Find service inside the services list
	auto res = this->services.find(ID);

	if (res == this->services.end())
	{
		scout << "Service " + ID + " is not loaded" << std::endl;
		return false;
	}

	auto pData = res->second;

    //shutdown service over the dds comunication layer
	CommandSendMsg msg;
	msg.name(pData->ID);
	msg.command("shutdown");
	this->ddsMonitor->sendMessage((void*) &msg);

    //wait a little for service to exit
	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(200));

	if(pData->proc->running()){
		scout << "### SERVICE IS STILL RUNNING WAITING A LITTLE TO GET PROPPER EXIT ###" << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		if(pData->proc->running()){
			scout << "### FORCING SERVICE " << pData->proc->id() << " TO EXIT ###" << std::endl;
			kill(pData->proc->id(), SIGKILL);
		}
	}

	pData->proc = nullptr;
	pData->dds_reader = nullptr;
	this->services.erase(pData->ID);

    return true;
}

ServiceLayer::Status ServiceLayer::shutdown()
{
	this->should_quit = true;

	setStatus(Status::STOP);

	return getStatus();
}

int ServiceLayer::numOfServices()
{
	return this->services.size();
}