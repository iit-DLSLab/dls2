// =============================================================================
// Includes
// =============================================================================
#include <memory>
#include <signal.h>
#include <iostream> // TODO temp
#include <unistd.h>
#include <cstring>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include "application_framework/hyq_app.hpp"
#include "application_framework/hardware_layer.hpp"
#include "application_framework/control_layer.hpp"

// TODO temp
#include "controller/dummy_controller.hpp"

#include "geometry/pose.hpp"
#include "todo.h"

// =============================================================================
// Forward Declarations
// =============================================================================

/// Pointer to application
std::shared_ptr<HyQApp> pApp;

void handle_args(int argc, char **argv);

// ============================== Signal Handling ==============================
struct sigaction sig_action;

/// Handler for segfaults during initialisiation
///
/// This handler is called before the application has been fully initialised
void initialisation_segfault_handler(int signum, siginfo_t *info, void *);

/// Handler for segfaults during application runtime
///
/// This handler is called if the application is initialised. It will call all
/// emergency stops and exit the program safely
void runtime_segfault_handler(int signum, siginfo_t *info, void *);

/// Change the name of a process for ease of monitoring inside of htop, ps etc
///
/// Linux only allocates a certain amount of space for the process name. Hence,
/// this function may truncate the name if it is too long for the alloted space
void change_process_name(char **argv, const std::string &name);

// =============================================================================
// Main Logic
// =============================================================================
TODO("HELLO")
int main(int argc, char **argv)
{
	TODO("Hello world")
	// Runtime Configuration
	handle_args(argc, argv);

	// Setup Signal Handling
	// sig_action.sa_sigaction = initialisation_segfault_handler;
	// sig_action.sa_flags = SA_SIGINFO;
	// sigaction(SIGSEGV, &sig_action, nullptr);

	// Create application
	pApp = std::make_shared<HyQApp>();

	const pid_t hardware_layer_pid = fork();
	if(hardware_layer_pid == -1)
	{
		// TODO fork failed, handle error
		return -1;

	}

	// if child process
	else if(hardware_layer_pid == 0)
	{
		// int argv0size = std::strlen(argv[0]);
		// std::strncpy(argv[0], "hardware layer", argv0size);
		// argv[0] = (char*)"hardware layer";
		change_process_name(argv, "hardware_layer");

		// prctl(PR_SET_NAME, "hardware_layer");
// while(true);
		// Child Process. Hardware process here
		std::shared_ptr<HardwareLayer> pHardwareLayer = std::make_shared<HardwareLayer>();
		pApp->addLayer(pHardwareLayer);

		// TODO run should return a status
		pApp->run();

		// Force segfault TODO remove
		// int *p = nullptr;
		// *p = 1;

		return 0;
	}

	const pid_t control_layer = fork();
	if(control_layer == -1)
	{
		// TODO fork failed, handle error
		return -1;
	}

	// if child process
	else if (control_layer == 0)
	{
		change_process_name(argv, "control_layer");
		std::shared_ptr<ControlLayer> pControlLayer = std::make_shared<ControlLayer>();
		std::shared_ptr<Dog> pDog;
		// std::shared_ptr<DummyController> pDummy_controller =
		// 	std::make_shared<DummyController>(pDog);
		// std::shared_ptr<DummyController> pDummy_controller =
		// 	std::make_shared<DummyController>();
		// pControlLayer->addController(pDummy_controller);
		pControlLayer->loadController("./libdummy_controller.so");
		pControlLayer->activateController("dummy_controller");
		pApp->addLayer(pControlLayer);

		// TODO run should return a status
		pApp->run();
		return 0;
	}

	// Wait for children to finish
	// Handle crashes in child processes
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
		if(child_pid == hardware_layer_pid)
		{
			std::cout << "child is hardware layer" << std::endl;
		}
		else if(child_pid == control_layer)
		{
			std::cout << "child is program layer" << std::endl;
		}
		if(WIFEXITED(status))
		{
			// TODO
			std::cout << "Child process exited normally with exit status " <<
				WEXITSTATUS(status) << std::endl;
		}
		if(WIFSIGNALED(status))
		{
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


	// Change Signal Handler to safely stop robot
	// sig_action.sa_sigaction = runtime_segfault_handler;
	// sig_action.sa_flags = SA_SIGINFO;
	// sigaction(SIGSEGV, &sig_action, nullptr);


	// Force segfault TODO remove
	// int *p = nullptr;
	// *p = 1;
	std::cout << "closing out" << std::endl;

	return 0;
}

// =============================================================================
// Helper Functions
// =============================================================================
void handle_args(int argc, char **argv)
{
	// TODO something useful here
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

// -----------------------------------------------------------------------------
// Signal Handler Functions
// -----------------------------------------------------------------------------
void initialisation_segfault_handler(int signum, siginfo_t *, void *)
{
	// TODO inspect info
	if(signum != SIGSEGV) return;
	std::cout << "Segfault during initialisation" << std::endl;

	exit(-1);
}

void runtime_segfault_handler(int signum, siginfo_t *, void *)
{
	// TODO inspect info
	if(signum != SIGSEGV) return;
	std::cout << "Segfault during runtime" << std::endl;
	pApp->panic();

	exit(-1);
}

