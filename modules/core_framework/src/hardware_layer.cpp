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
#include "dls2/core_framework/hardware_layer.hpp"

#include "dls2/class_loader.hpp"
#include "dls2/core_framework/options.hpp"


using namespace dls;

HardwareLayer::HardwareLayer(std::string ID_) 
	: AppLayer(ID_)
{
    ddsMonitor = new dls::DDSWriter(
		"HardwareLayer::monitor",
		dls::domains::hardwares,
		dls::topics::command_send
	);

    command_manager.addCommand<std::string>
	(
		"loadHardware",
		"Load thel HAL of a robot",
		std::function<bool(std::string)>([&](std::string type)->bool
        {
			return this->activateHardware(type);
        }),
		{{0,1}},
		true
	);

    command_manager.addCommand<>
	(
		"unloadHardware",
		"Terminates the HAL of the robot",
		std::function<bool(std::string)>([&](std::string name)->bool
        {
            return this->deactivateHardware(name);
		}),
		{{1,0}},
		true
	);
}

HardwareLayer::~HardwareLayer()
{
	scout << "#### HARDWARE INTERFACE OFF ####" << std::endl;
}

HardwareLayer::Status HardwareLayer::run()
{
	// TODO("Check status of all components in the control layer, take corrective actions if requred")
	setStatus(Status::RUNNING);

	while(!this->shouldQuit())
	{
		// TODO this loop should be used to verify the state of the layer and all its controllers		
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
	}
	return getStatus();
}


bool HardwareLayer::activateHardware(const std::string &robotType)
{
    std::shared_ptr<AppData> pData;

    for(long unsigned int i = 0; i <= hardwares.size(); i++){
        std::string ID = robotType + "_" + std::to_string(i);
        if(this->hardwares.find(ID) == this->hardwares.end())
		{
			pData = std::make_shared<AppData>(ID);
            break;
		}
    }   

	{
		// std::lock_guard<std::mutex> lock(this->hardwares_mutex);

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

		pData->proc = std::make_shared<boost::process::child>(std::vector<std::string>({
			child_process_launcher,
			pData->getID(),
			robotType + "_hal",
			"hardware",
			robotType,
            "live"
		}));

		if (pData->proc == nullptr){
			scout << "Controller process failed to launch" << std::endl;
			return false;
		}

        if (!pData->proc->running()){
			scout << "Controller process failed to launch" << std::endl;
			return false;
		}

		scout << "HARDWARE LAYER OF " << pData->getID() << " IS ON" <<  std::endl;

		this->hardwares.emplace(pData->getID(), pData);
	}

	return true;
}

bool HardwareLayer::deactivateHardware(std::shared_ptr<AppData> pData)
{
	std::lock_guard<std::mutex> lock(this->hardwares_mutex);

    //shutdown controller over the dds comunication layer
	CommandSendMsg msg;
	msg.name(pData->getID());
	msg.command("shutdown");
	this->ddsMonitor->sendMessage((void*) &msg);

    //wait a little for hardware to exit
	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(200));

	if(pData->proc->running()){
		scout << "### HARDWARE IS STILL RUNNING WAITING A LITTLE TO GET PROPPER EXIT ###" << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		if(pData->proc->running()){
			scout << "### FORCING HARDWARE " << pData->proc->id() << " TO EXIT ###" << std::endl;
			kill(pData->proc->id(), SIGKILL);
		}
	}

	pData->proc = nullptr;
	this->hardwares.erase(pData->getID());

    return false;
}

bool HardwareLayer::deactivateHardware(const std::string &ID)
{
	decltype(this->hardwares.find(ID)) pair_it;
	{
		std::lock_guard<std::mutex> lock(this->hardwares_mutex);
		pair_it = this->hardwares.find(ID);

		if(pair_it == this->hardwares.end()) 
			return false;
	}

    return this->deactivateHardware(pair_it->second);
}

HardwareLayer::Status HardwareLayer::shutdown()
{
	this->should_quit = true;

	setStatus(Status::STOP);

	return getStatus();
}