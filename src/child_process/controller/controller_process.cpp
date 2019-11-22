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

std::shared_ptr<dls::Controller> pController;
void signal_handler(int signal);

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <controller_name>" << std::endl;
		std::cerr << "argc: " << argc << std::endl;
		exit(EXIT_FAILURE);
	}

	try // this is a quick hack to first check local directory for library
	{
		pController =
			dls::ClassLoader::loadClass<dls::Controller>(std::string(LIBRARY_PROCESS_PATH "./lib") + argv[1] + ".so");
	}
	catch(const std::exception&)
	{
		try
		{
			pController =
				dls::ClassLoader::loadClass<dls::Controller>(std::string(LIBRARY_PROCESS_PATH "lib") + argv[1] + ".so");
		}
		catch(const std::exception&)
		{
			DMSG("ADFSLFSDJLSDJFLSJFD");
			dls::logging::cfatal << "Controller not found" << dls::logging::endl;
			exit((int)dls::Controller::Status::FATAL_ERROR);
		}
	}

	std::signal(SIGTERM, signal_handler);
	dls::logging::clog << "controller loaded" << dls::logging::endl;
	pController->run();

	return static_cast<int>(pController->getStatus());
}

void signal_handler(int signal)
{
	if(signal == SIGTERM)
	{
		std::stringstream ss;
		ss << pController->getID();
		ss << " received kill request";
		dls::logging::clog << ss.str() << dls::logging::endl;

		pController->stop();
		exit(static_cast<int>(pController->getStatus()));
	}
}

// If this class isn't delcared, the linker will link the whole class away, then
// the call to loadClass will fail
class VoidController : public dls::Controller
{
public:
	VoidController
	(
	) :
		Controller
		(
			nullptr,
			"dummy_controller",
			std::chrono::duration<double>(1),
			dls::ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
		)
	{ }

	~VoidController() = default;

	void run(const std::chrono::system_clock::time_point &time)override
	{
		time.time_since_epoch();
	};

	Status eStop() override { return getStatus(); }
}voidController;
