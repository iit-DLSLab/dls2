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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <csignal>
#include <sstream>

#include "util/class_loader.hpp"
#include "controller/controller.hpp"
#include "util/debug/debug.hpp"
#include "util/log/log.hpp"
#include "util/debug/debug.hpp"

#include "path_prefixes/path_prefixes.hpp"
#include "util/time/time.hpp"

using namespace dls;
std::shared_ptr<Controller> pController;
void signal_handler(int signal);

auto process_handle = "controller_child_process_launcher";

int main(int argc, char **argv)
{
	Time::set_use_simulated_time(true);
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <controller_name>" << std::endl;
		std::cerr << "argc: " << argc << std::endl;
		exit(EXIT_FAILURE);
	}

	try // this is a quick hack to first check local directory for library
	{
		pController =
			ClassLoader::loadClass<Controller>(std::string(LIBRARY_PROCESS_PATH "./lib") + argv[1] + ".so");
	}
	catch(const std::exception&)
	{
		try
		{
			pController =
				ClassLoader::loadClass<Controller>(std::string(LIBRARY_PROCESS_PATH "lib") + argv[1] + ".so");
		}
		catch(const std::exception&)
		{
			logging::cfatalstream s(process_handle);
			s << "Controller not found" << std::endl;
			exit((int)Controller::Status::FATAL_ERROR);
		}
	}

	std::signal(SIGTERM, signal_handler);
	logging::coutstream s(process_handle);
	s << "controller loaded" << std::endl;
	pController->run();

	return static_cast<int>(pController->getStatus());
}

void signal_handler(int signal)
{
	if(signal == SIGTERM)
	{
		pController->stop();

		logging::coutstream s(process_handle);
		std::cout << pController->getID() << " received kill request" << std::endl;
		s << pController->getID() << " received kill request";
		auto exit_status = pController->getStatus();
		pController = nullptr;
		exit(static_cast<int>(exit_status));
	}
}

// If this class isn't delcared, the linker will link the whole class away, then
// the call to loadClass will fail
class VoidController : public Controller
{
public:
	VoidController
	(
	) :
		Controller
		(
			nullptr,
			"void_controller",
			std::chrono::duration<double>(1),
			ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
		)
	{ }

	~VoidController() = default;

	void run(const std::chrono::system_clock::time_point &time)override
	{
		time.time_since_epoch();
	};

	Status eStop() override { return getStatus(); }
}voidController;
