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
#include "gait_generator/gait_generator.hpp"
#include "util/debug/debug.hpp"
#include "path_prefixes/path_prefixes.hpp"
#include "util/log/log.hpp"
#include "util/debug/debug.hpp"

#include "util/time/time.hpp"
using namespace dls;
std::shared_ptr<GaitGenerator> pGaitGenerator;
void signal_handler(int signal);

auto process_handle = "controller_child_process_launcher";

int main(int argc, char **argv)
{
	Time::set_use_simulated_time(true);
	DMSG("USING SIMULATED TIME");
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <gait_generator_name>" << std::endl;
		std::cerr << "argc: " << argc << std::endl;
		exit(EXIT_FAILURE);
	}

	try // this is a quick hack to first check local directory for library
	{
		pGaitGenerator =
			ClassLoader::loadClass<GaitGenerator>(std::string(LIBRARY_PROCESS_PATH "./lib") + argv[1] + ".so");
	}
	catch(const std::exception&)
	{
		try
		{
			pGaitGenerator =
				ClassLoader::loadClass<GaitGenerator>(std::string(LIBRARY_PROCESS_PATH "lib") + argv[1] + ".so");
		}
		catch(const std::exception&)
		{
			DMSG("ADFSLFSDJLSDJFLSJFD");
			logging::cfatalstream s(process_handle);
			s << "Gait generator not found" << std::endl;
			exit((int)GaitGenerator::Status::FATAL_ERROR);
		}
	}

	std::signal(SIGTERM, signal_handler);
	logging::coutstream s(process_handle);
	s << "Gait generator loaded" << std::endl;
	pGaitGenerator->run();

	return static_cast<int>(pGaitGenerator->getStatus());
}

void signal_handler(int signal)
{
	if(signal == SIGTERM)
	{
		pGaitGenerator->stop();

		logging::coutstream s(process_handle);
		s << pGaitGenerator->getID() << " received kill request";

		exit(static_cast<int>(pGaitGenerator->getStatus()));
	}
}

// If this class isn't delcared, the linker will link the whole class away, then
// the call to loadClass will fail
class VoidGaitGenerator : public GaitGenerator
{
public:
	VoidGaitGenerator
	(
	) :
		GaitGenerator
		(
			nullptr,
			"dummy_controller",
			std::chrono::duration<double>(1)
		)
	{ }

	~VoidGaitGenerator() = default;

	void run(const std::chrono::system_clock::time_point &time)override
	{
		time.time_since_epoch();
	};

	Status eStop() override { return getStatus(); }
}voidGaitGenerator;
