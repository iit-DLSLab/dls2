// =============================================================================
// Includes
// =============================================================================
#include "todo.h"
#include <memory>
#include <signal.h>
#include <unistd.h>
#include <cstring>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include "application_framework/hyq_app.hpp"
#include "application_framework/hardware_layer.hpp"
#include "application_framework/control_layer.hpp"
#include "application_framework/estimation_layer.hpp"
#include "application_framework/log_layer.hpp"

#include "util/debug/debug.hpp"

TODO("temporary include")
#include "controller/dummy_controller.hpp"
TODO("temporary incude")
#include <thread>

#include "geometry/pose.hpp"

#include "util/log/log.hpp"

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
	const pid_t hardware_layer_pid = fork();
	if(hardware_layer_pid == -1)
	{
		TODO("handle error of fork")
		return -1;

	}
	else if(hardware_layer_pid == 0)
	{
		change_process_name(argv, "hardware_layer");
		std::shared_ptr<HardwareLayer> pHardwareLayer = std::make_shared<HardwareLayer>();
		pApp->addLayer(pHardwareLayer);

		TODO("Run should return a status")
		pApp->run();
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
		pApp->run();

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
		pApp->run();

		// ============================== Temporary Thing =======================
		TODO("This is temporary to simulate user input")
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		// std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(3000));
		// pControlLayer->loadController("./libdummy_controller.so");
		// pControlLayer->loadGaitGenerator("./libdummy_gait_generator.so");
		// std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(3000));
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		pControlLayer->activateController("dls_dummy_controller");
		pControlLayer->activateGaitGenerator("dls_dummy_gait_generator");
		// pEstimationLayer->loadEstimator("./libdummy_estimator.so");
		// pEstimationLayer->activateEstimator("Dummy Estimator");

		// std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(30000));
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(9000));

		return 0;
	}
	// ======================== Monitor Child Processes ========================
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
	logging::cout << "hello world" << logging::endl;
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

		DMSG("Child pid: " << child_pid);
		// if(child_pid == hardware_layer_pid)
		// {
		// 	DMSG("child is hardware layer");
		// }
		// else if(child_pid == control_layer)
		// {
		// 	DMSG("child is program layer");
		// }
		/*else*/ if(child_pid == log_layer_pid)
		{
			DMSG("child is log layer");
		}
		if(WIFEXITED(status))
		{
			DMSG("Child process exited normally with exit status " <<
				WEXITSTATUS(status));
		}
		if(WIFSIGNALED(status))
		{
			TODO("Handle case where child process crashed")
			DMSG("Child process exited by signal " << WTERMSIG(status));

			#ifdef WCOREDUMP
			if(WCOREDUMP(status)) // not available on all unix implementations
			{
				DMSG("Child process had a core dump");
			}
			#endif
		}
		// Ignoring WIFSTOPPED, WSTOPSIG, WIFCONTINUED
	}

	DMSG("closing out");

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
