// =============================================================================
// Includes
// =============================================================================
#include <memory>
#include <signal.h>
#include <iostream> // TODO temp
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "application_framework/hyq_app.hpp"
#include "application_framework/hardware_layer.hpp"
#include "application_framework/control_layer.hpp"

// TODO temp
#include "controller/dummy_controller.hpp"

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

// =============================================================================
// Main Logic
// =============================================================================
int main(int argc, char **argv)
{
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
		std::shared_ptr<ControlLayer> pControlLayer = std::make_shared<ControlLayer>();
		std::shared_ptr<Dog> pDog;
		std::shared_ptr<DummyController> pDummy_controller =
			std::make_shared<DummyController>(pDog, "dummy", std::chrono::duration<double>(1));
		pControlLayer->addController(pDummy_controller);
		pApp->addLayer(pControlLayer);

		// TODO run should return a status
		pApp->run();
		return 0;
	}

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

		// switch(child_pid)
		// {
		// 	case hardware_layer_pid:
		// 		break;
		// 	case control_layer:
		// 		break;
		// }
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
