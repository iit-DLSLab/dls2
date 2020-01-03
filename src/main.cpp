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
// =============================================================================
// Includes
// =============================================================================
#include "todo.h"
#include <memory>
#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include "application_framework/hyq_app.hpp"
#include "application_framework/hardware_layer.hpp"
#include "application_framework/control_layer.hpp"
#include "application_framework/estimation_layer.hpp"
#include "application_framework/log_layer.hpp"
#include "application_framework/console_layer.hpp"

#include "util/debug/debug.hpp"
#include "util/log/log.hpp"

#include <thread>

#include "geometry/pose.hpp"

#include "util/log/log.hpp"

using namespace dls;
// =============================================================================
// Forward Declarations
// =============================================================================

/// Pointer to application
std::shared_ptr<HyQApp> pApp;

bool handle_args(int argc, char **argv);

/// Change the name of a process for ease of monitoring inside of htop, ps etc
///
/// Linux only allocates a certain amount of space for the process name. Hence,
/// this function may truncate the name if it is too long for the alloted space
void change_process_name(char **argv, const std::string &name);

// =============================================================================
// Main Logic
// =============================================================================
int main(int argc, char **argv)
{
	#ifndef NDEBUG
		std::cout << "Debug build" << std::endl;
		std::cout << "Version: " << PROJECT_MAJOR << "."
			<< PROJECT_MINOR << "." << PROJECT_PATCH << std::endl;
	#endif
	// Runtime Configuration
	bool only_start_log = handle_args(argc, argv);

	// Create application
	pApp = std::make_shared<HyQApp>();

	// ============================= Console Layer =============================
	const pid_t console_layer_pid = fork();
	if(console_layer_pid == -1)
	{
		TODO("HANDE ERROR ON FORK")
		return -1;
	}
	else if(console_layer_pid == 0)
	{
		change_process_name(argv, "console_layer");
		std::shared_ptr<ConsoleLayer> pConsoleLayer = std::make_shared<ConsoleLayer>();
		pApp->addLayer(pConsoleLayer);
		if (!only_start_log) pApp->run();

		return 0;
	}
	// Give the console time to load its subscribers. If the fastrtps
	// configuration can be improved, this sleep will no longer be necessary
	std::this_thread::sleep_for(std::chrono::seconds(1));


	// ========================= Start Hardware Layer ==========================
	const pid_t hardware_layer_pid = fork();
	if(hardware_layer_pid == -1)
	{
		TODO("handle error of fork")
		return -1;

	}
	else if(hardware_layer_pid == 0)
	{
		if(!only_start_log)
		{
			change_process_name(argv, "hardware_layer");
			std::shared_ptr<HardwareLayer> pHardwareLayer = std::make_shared<HardwareLayer>();
			pApp->addLayer(pHardwareLayer);

			TODO("Run should return a status")
			pApp->run();
		}
		while(true);

		return 0;
	}
	// =============================== Log Layer ===============================
	const pid_t log_layer_pid = fork();
	if(log_layer_pid == -1)
	{
		TODO("Handle error on fork")
		return -1;
	}
	else if(log_layer_pid == 0)
	{
		change_process_name(argv, "log_layer");
		std::shared_ptr<LogLayer> pLogLayer = std::make_shared<LogLayer>();
		pApp->addLayer(pLogLayer);
		if (only_start_log) pApp->run();

		return 0;
	}

	// ========================== Start Control Layer ==========================
	const pid_t control_layer = fork();
	if(control_layer == -1)
	{
		TODO("Fork failed, handle error")
		return -1;
	}
	else if (control_layer == 0)
	{
		change_process_name(argv, "control_layer");
		std::shared_ptr<ControlLayer> pControlLayer = std::make_shared<ControlLayer>();
		TODO("move this to separate process")
		// std::shared_ptr<EstimationLayer> pEstimationLayer = std::make_shared<EstimationLayer>();

		pApp->addLayer(pControlLayer);
		// pApp->addLayer(pEstimationLayer);

		TODO("Run should return a status")
		TODO("Run should be blocking")
		if (!only_start_log) pApp->run();

		// ============================== Temporary Thing =======================
		// TODO("This is temporary to simulate user input")
		// std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		// // std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(3000));
		// // pControlLayer->loadController("./libdummy_controller.so");
		// // pControlLayer->loadGaitGenerator("./libdummy_gait_generator.so");
		// // std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(3000));
		// std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		// pControlLayer->activateController("dls_dummy_controller");
		// pControlLayer->activateGaitGenerator("dls_dummy_gait_generator");
		// // pEstimationLayer->loadEstimator("./libdummy_estimator.so");
		// // pEstimationLayer->activateEstimator("Dummy Estimator");

		// // std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(30000));
		// std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(9000));

		return 0;
	}
	// ======================== Monitor Child Processes ========================

	logging::coutstream s("framework_monitor");
	logging::cfatalstream sfatal("framework_monitor");
	while(true)
	{
		std::stringstream ss;
		int status;
		pid_t child_pid = wait(&status);
		if(child_pid == -1)
		{
			// if no more children left
			if(errno == ECHILD)
			{
				break;
			}
		}

		if(child_pid == hardware_layer_pid)
		{
			ss << "Hardware layer exited ";
		}
		else if(child_pid == control_layer)
		{
			ss << "Control layer exited ";
		}
		else if(child_pid == log_layer_pid)
		{
			ss << "log layer exited ";
		}
		else if(child_pid == console_layer_pid)
		{
			ss << "console layer exited ";
		}
		if(WIFEXITED(status))
		{
			ss << " normally with exit status " << WEXITSTATUS(status) << std::endl;
			// DMSG(WEXITSTATUS(status));
		}
		if(WIFSIGNALED(status))
		{
			TODO("Handle case where child process crashed")
			ss << " by signal " << WTERMSIG(status) << std::endl;
			// DMSG(WTERMSIG(status));

			#ifdef WCOREDUMP
			if(WCOREDUMP(status)) // not available on all unix implementations
			{
				sfatal << "Child had a core dump" << std::endl;
			}
			#endif
		}
		s << ss.str() << std::endl;
		// Ignoring WIFSTOPPED, WSTOPSIG, WIFCONTINUED
	}
	return 0;
}

// =============================================================================
// Helper Functions
// =============================================================================
bool handle_args(int argc, char **argv)
{
	TODO("something useful here")
	if (argc==2)
	{
		if (strncmp (argv[1],"-l",2) == 0)
		{
			return true;
		}
	}
	return false;
}




void change_process_name(char **argv, const std::string &name)
{
	// change info in /proc/$pid/cmdline
	int alloted_space = std::strlen(argv[0]);
	std::strncpy(argv[0], name.c_str(), alloted_space);

	// change info in /proc/$pid/status
	prctl(PR_SET_NAME, name.c_str());
}
