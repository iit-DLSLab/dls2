
#include "dls2/core_framework/service_layer.hpp"

#include "dls2/class_loader.hpp"
#include "dls2/core_framework/options.hpp"

using namespace dls;


ServiceLayer::ServiceLayer(std::string ID, const std::string& robot_name) 
	: Layer(ID, 500)
	, ddsMonitor(std::make_shared<dls::DDSWriter>(
		"ServiceLayer::monitor",
		dls::domains::services,
		dls::topics::command_send
	))
	, action_client_stock(robot_name)
	, robot_name(robot_name)
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
	
	command_manager.addCommand<std::string>
	(
		"loadAction",
		"Load an action",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			return this->loadAction(type);
        }),
		{},
		true
	);

	command_manager.addCommand<std::string>
	(
		"unloadAction",
		"Remove action",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			if(this->unloadAction(type))
			{
				return true;
			}

			return false;
		}),
		{},
		true
	);
}

ServiceLayer::~ServiceLayer()
{
	scout_sys << "#### SERVICE LAYER OFF ####" << std::endl;
}

void ServiceLayer::monitor(){}

void ServiceLayer::close(){
	std::vector<std::string> keys;
	for(auto pair : this->services)
		keys.push_back(pair.first);
	
	for(auto key : keys)
		this->unloadService(key);
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

		if (pData->proc == nullptr){
			scout_err << "Service " << lib_name <<" failed to launch: nullptr" << std::endl;
			return false;
		}
		else if (pData->proc->wait_for(std::chrono::duration<double, std::milli>(1500))){
			scout_err << "Service " << lib_name <<" failed to launch: expired timeout" << std::endl;
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
	command_manager.callCommand("shutdown", {}, ID);

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

bool ServiceLayer::loadAction(const std::string& action_name)
{
	if(this->actions.find(action_name) != this->actions.end())
	{
		scout_err << "action " + action_name + " already loaded" << std::endl;
		return false;
	}

    std::shared_ptr<AppData> pData = std::make_shared<AppData>(action_name);
    
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
			action_name,
			"action",
			this->robot_name
		}));

		if (pData->proc == nullptr){
			scout_err << "Action " << action_name <<" failed to launch: nullptr" << std::endl;
			return false;
		}
		else if (pData->proc->wait_for(std::chrono::duration<double, std::milli>(1500))){
			scout_err << "Action " << action_name <<" failed to launch: expired timeout" << std::endl;
			return false;
		}

		scout_sys << "ACTION " << pData->getID() << " IS ON" <<  std::endl;

		this->actions.emplace(pData->getID(), pData);
	}

	return true;	
}

bool ServiceLayer::unloadAction(const std::string &action_name)
{
	// Find action inside the action list
	auto res = this->actions.find(action_name);

	if (res == this->actions.end())
	{
		scout_err << "Action " + action_name + " is not loaded" << std::endl;
		return false;
	}

	auto pData = res->second;

    //shutdown action over the dds comunication layer
	command_manager.callCommand("shutdown", {}, action_name);
    // wait a little for action to exit
	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));

	if(pData->proc->running()){
		scout_warn << "### ACTION " << action_name <<" IS STILL RUNNING WAITING A LITTLE TO GET PROPER EXIT ###" << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		if(pData->proc->running()){
			scout_warn << "### FORCING ACTION " << action_name <<" TO EXIT ###" << std::endl;
			kill(pData->proc->id(), SIGKILL);
		}
	}
	else
	{
		scout_sys << "Action " + action_name + " is unloaded" << std::endl;
	}

	pData->proc = nullptr;
	this->actions.erase(action_name);
    return true;
}

int ServiceLayer::numOfServices()
{
	return this->services.size();
}