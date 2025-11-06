
#include "dls2/core_framework/service_layer.hpp"
#include "dls2/util/utils.hpp"

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
		"loadDataVisualizer",
		"Loads a data visualizer",
		std::function<bool(std::string)>([&](std::string s)->bool
		{
			return this->loadDataVisualizer(s);
		}),
		{{0,1},{1,1}},
		true
	);
	
	command_manager.addCommand<>
	(
		"unloadDataVisualizer",
		"Unloads a data visualizer",
		std::function<bool(std::string)>([&](std::string s)->bool
		{
			if(this->unloadDataVisualizer(s))
			{
				if(this->numOfDataVisualizers() == 0)
					return true;
			}

			return false;
		}),
		{{1,0}},
		true
	);

	command_manager.addCommand<std::string>
	(
		"loadProcedure",
		"Load a procedure",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			return this->loadProcedure(type);
        }),
		{},
		true
	);

	command_manager.addCommand<std::string>
	(
		"unloadProcedure",
		"Remove procedure",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			if(this->unloadProcedure(type))
			{
				return true;
			}

			return false;
		}),
		{},
		true
	);

	command_manager.addCommand<std::string>
	(
		"loadTask",
		"Load a task",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			return this->loadTask(type);
        }),
		{},
		true
	);

	command_manager.addCommand<std::string>
	(
		"unloadTask",
		"Remove task",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			if(this->unloadTask(type))
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
	for(auto pair : this->services)
		this->unloadService(pair.first);
	for(auto pair : this->data_visualizers_)
		this->unloadDataVisualizer(pair.first);
	for(auto pair : this->tasks)
		this->unloadTask(pair.first);
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

	bool unloaded = false;
	if(!utils::wait(std::function<bool()>([&](){
			if(pData->proc->running()){
				return false;
			}
			return true;
		}), 2000, 10, unloaded)){
		scout_warn << "### FORCING SERVICE " << ID << " EXIT ###" << std::endl;
		kill(pData->proc->id(), SIGKILL);		
	}

	scout_sys << "Service " + ID + " is unloaded" << std::endl;
	pData->proc = nullptr;
	this->services.erase(ID);
    return true;
}

bool ServiceLayer::loadDataVisualizer(const std::string& lib_name)
{
	if(this->data_visualizers_.find(lib_name) != this->data_visualizers_.end())
	{
		scout_err << "Data visualizer " + lib_name + " already loaded" << std::endl;
		return false;
	}

    std::shared_ptr<AppData> pData = std::make_shared<AppData>(lib_name);
    
	{
		// std::lock_guard<std::mutex> lock(this->data_visualizers_mutex_);

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
			"data_visualizer",
			robot_name
		}));

		if (pData->proc == nullptr){
			scout_err << "Data visualizer " << lib_name <<" failed to launch: nullptr" << std::endl;
			return false;
		}

		this->data_visualizers_.emplace(pData->getID(), pData);
	}

	return true;
}

bool ServiceLayer::unloadDataVisualizer(const std::string ID)
{
	// Find data visualizers inside the services list
	auto res = this->data_visualizers_.find(ID);

	if (res == this->data_visualizers_.end())
	{
		scout_err << "Data visualizer " + ID + " is not loaded" << std::endl;
		return false;
	}

	auto pData = res->second;

    //shutdown data visualizer over the dds comunication layer
	command_manager.callCommand("shutdown", {}, ID);

	bool unloaded = false;
	if(!utils::wait(std::function<bool()>([&](){
			if(pData->proc->running()){
				return false;
			}
			return true;
		}), 2000, 10, unloaded)){
		scout_warn << "### FORCING DATA VISUALIZER " << ID << " EXIT ###" << std::endl;
		kill(pData->proc->id(), SIGKILL);		
	}

	scout_sys << "Data visualizer " + ID + " is unloaded" << std::endl;
	pData->proc = nullptr;
	this->data_visualizers_.erase(ID);
    return true;
}

bool ServiceLayer::loadTask(const std::string& name)
{
	if(this->tasks.find(name) != this->tasks.end())
	{
		scout_err << "task " + name + " already loaded" << std::endl;
		return false;
	}

    std::shared_ptr<AppData> pData = std::make_shared<AppData>(name);
    
	// launch the procedure
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
		name,
		"task",
		this->robot_name
	}));

	if (pData->proc == nullptr){
		std::cout << "Task " << name <<" failed to launch: nullptr" << std::endl;
		return false;
	}
	
	pData->proc->detach();
	
	this->tasks.emplace(pData->getID(), pData);

	return true;	
}

bool ServiceLayer::unloadTask(const std::string &name)
{
	// Find task inside the task list
	auto res = this->tasks.find(name);

	if (res == this->tasks.end())
	{
		scout_err << "Task " + name + " is not loaded" << std::endl;
		return false;
	}

	auto pData = res->second;

    //shutdown task over the dds comunication layer
	command_manager.callCommand("shutdown", {}, name);

	bool unloaded = false;
	if(!utils::wait(std::function<bool()>([&](){
			if(pData->proc->running()){
				return false;
			}
			return true;
		}), 2000, 10, unloaded)){
		std::cout << "### FORCING TASK " << name << " EXIT ###" << std::endl;
		kill(pData->proc->id(), SIGKILL);		
	}
	
	std::cout << "Task " + name + " is unloaded" << std::endl;
	
	pData->proc = nullptr;
	this->tasks.erase(name);
    return true;
}


int ServiceLayer::numOfServices()
{
	return this->services.size();
}

int ServiceLayer::numOfDataVisualizers()
{
	return this->data_visualizers_.size();
}