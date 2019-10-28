// =============================================================================
// Includes
// =============================================================================
#include "todo.h"
#include <memory>
#include <signal.h>
TODO("remove iostream include")
#include <iostream>
#include <unistd.h>
#include <cstring>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include "application_framework/hyq_app.hpp"
#include "application_framework/hardware_layer.hpp"
#include "application_framework/control_layer.hpp"

TODO("temporary include")
#include "controller/dummy_controller.hpp"
TODO("temporary incude")
#include <thread>

#include "geometry/pose.hpp"

// =============================================================================
// Forward Declarations
// =============================================================================

/// Pointer to application
std::shared_ptr<HyQApp> pApp;

void handle_args(int argc, char **argv);

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
	// Runtime Configuration
	handle_args(argc, argv);

	// Create application
	pApp = std::make_shared<HyQApp>();

	// ========================= Start Hardware Layer ==========================
	TODO("Don't remove this")
	TODO("This was commented out also at the bottom")
	// const pid_t hardware_layer_pid = fork();
	// if(hardware_layer_pid == -1)
	// {
	// 	TODO("handle error of fork")
	// 	return -1;

	// }
	// else if(hardware_layer_pid == 0)
	// {
	// 	change_process_name(argv, "hardware_layer");
	// 	std::shared_ptr<HardwareLayer> pHardwareLayer = std::make_shared<HardwareLayer>();
	// 	pApp->addLayer(pHardwareLayer);

	// 	TODO("Run should return a status")
	// 	pApp->run();

	// 	return 0;
	// }

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

		pApp->addLayer(pControlLayer);

		std::cout << "running control layer" << std::endl;
		TODO("Run should return a status")
		pApp->run();

		// ============================== Temporary Thing =======================
		TODO("This is temporary to simulate user input")
		std::cout << "waiting for user input" << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(3000));
		std::cout << "user starting dummy controller" << std::endl;
		pControlLayer->loadController("./libdummy_controller.so");
		pControlLayer->activateController("dummy_controller");

		std::cout << "waiting for contol layer to end" << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(3000));
		std::cout << "Forcing success of control layer" << std::endl;
		pControlLayer->setStatus(AppLayer::Status::SUCCESS);

		return 0;
	}

	// ======================== Monitor Child Processes ========================
	while(true)
	{
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

		std::cout << "child pid: " << child_pid << std::endl;
		// if(child_pid == hardware_layer_pid)
		// {
		// 	std::cout << "child is hardware layer" << std::endl;
		// }
		/*else*/ if(child_pid == control_layer)
		{
			std::cout << "child is program layer" << std::endl;
		}
		if(WIFEXITED(status))
		{
			std::cout << "Child process exited normally with exit status " <<
				WEXITSTATUS(status) << std::endl;
		}
		if(WIFSIGNALED(status))
		{
			TODO("Handle case where child process crashed")
			std::cout << "Child process exited by signal " <<
				WTERMSIG(status) << std::endl;

			#ifdef WCOREDUMP
			if(WCOREDUMP(status)) // not available on all unix implementations
			{
				std::cout << "Child process had a core dump" << std::endl;
			}
			#endif
		}
		std::cout << "======" << std::endl;
		// Ignoring WIFSTOPPED, WSTOPSIG, WIFCONTINUED
	}

	std::cout << "closing out" << std::endl;

	return 0;
}

// =============================================================================
// Helper Functions
// =============================================================================
void handle_args(int argc, char **argv)
{
	TODO("something useful here")
	argc += 1;
	char *a = argv[0];
	a = argv[1];
	a += 1;
}

void change_process_name(char **argv, const std::string &name)
{
	// change info in /proc/$pid/cmdline
	int alloted_space = std::strlen(argv[0]);
	std::strncpy(argv[0], name.c_str(), alloted_space);

	// change info in /proc/$pid/status
	prctl(PR_SET_NAME, name.c_str());
}
