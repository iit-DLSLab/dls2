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

std::shared_ptr<dls::GaitGenerator> pGaitGenerator;
void signal_handler(int signal);

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <gait_generator_name>" << std::endl;
		std::cerr << "argc: " << argc << std::endl;
		exit(EXIT_FAILURE);
	}

	try // this is a quick hack to first check local directory for library
	{
		pGaitGenerator =
			dls::ClassLoader::loadClass<dls::GaitGenerator>(std::string(LIBRARY_PROCESS_PATH "./lib") + argv[1] + ".so");
	}
	catch(const std::exception&)
	{
		try
		{
			pGaitGenerator =
				dls::ClassLoader::loadClass<dls::GaitGenerator>(std::string(LIBRARY_PROCESS_PATH "lib") + argv[1] + ".so");
		}
		catch(const std::exception&)
		{
			DMSG("ADFSLFSDJLSDJFLSJFD");
			dls::logging::cfatal << "Gait generator not found" << dls::logging::endl;
			exit((int)dls::GaitGenerator::Status::FATAL_ERROR);
		}
	}

	std::signal(SIGTERM, signal_handler);
	dls::logging::clog << "Gait generator loaded" << dls::logging::endl;
	pGaitGenerator->run();

	return static_cast<int>(pGaitGenerator->getStatus());
}

void signal_handler(int signal)
{
	if(signal == SIGTERM)
	{
		std::stringstream ss;
		ss << pGaitGenerator->getID();
		ss << " received kill request";
		dls::logging::clog << ss.str() << dls::logging::endl;

		pGaitGenerator->stop();
		exit(static_cast<int>(pGaitGenerator->getStatus()));
	}
}

// If this class isn't delcared, the linker will link the whole class away, then
// the call to loadClass will fail
class VoidGaitGenerator : public dls::GaitGenerator
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
