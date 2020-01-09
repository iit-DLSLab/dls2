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
#include "command/command.hpp"

#include <thread>

#include "geometry/pose.hpp"

#include "util/log/log.hpp"

using namespace dls;
// =============================================================================
// Forward Declarations
// =============================================================================

/// Pointer to application
///
std::shared_ptr<HyQApp> pApp;


/// Child proccess data
///
struct ChildData
{
	ChildData(const std::string &name_, pid_t pid_) :
		name(name_), pid(pid_)
	{ }

	/// Name of procses as reported to users
	///
	std::string name;

	/// PID of process
	///
	pid_t pid;
};

std::map<pid_t, ChildData> child_datas;

// =============================================================================
// Prtototypes
// =============================================================================
bool handle_args(int argc, char **argv);

/// Change the name of a process for ease of monitoring inside of htop, ps etc
///
/// Linux only allocates a certain amount of space for the process name. Hence,
/// this function may truncate the name if it is too long for the alloted space
void change_process_name(char **argv, const std::string &name);

/// Forks the process and launches the application containing a layer specified
/// by the template argument
///
template <class layer_t>
void forkLayer(const std::string &process_name, char **argv);

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

	// Ignore cntrl-C
	//
	// To exit the framework, an exit command is registered later
	signal(SIGINT, SIG_IGN);

	// Create application
	pApp = std::make_shared<HyQApp>();

	// Launch layers
	if(only_start_log)
	{
		forkLayer<LogLayer>("log_layer", argv);
	}
	else
	{
		forkLayer<ConsoleLayer>("console_layer", argv);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		forkLayer<HardwareLayer>("hardware_layer", argv);
		forkLayer<ControlLayer>("control_layer", argv);
	}

	logging::coutstream outstream("framework_monitor");
	logging::cfatalstream sfatal("framework_monitor");

	// Register an exit command to leave the framework
	CommandManager command_manager;
	command_manager.addCommand<void, ARGVOID>
	(
		"HyQApp_server",
		"exit",
		"exits the framework",
		std::function<void(ARGVOID)>
		(
			[&](ARGVOID)
			{
				for(const auto &el : child_datas)
				{
					kill(el.second.pid, SIGTERM);
				}
			}
		)
	);

	command_manager.addCommand<void, ARGVOID>
	(
		"HyQApp_server",
		"void",
		"voids",
		std::function<void(ARGVOID)>
		(
			[&](ARGVOID)
			{
				std::cout << "voided" << std::endl;
			}
		)
	);

	// Monitor the child processes
	//
	// loop is exited when there are no more children
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

		auto it = child_datas.find(child_pid);
		if(it != child_datas.end())
		{
			ss << it->second.name << " ";
			// Ignoring WIFSTOPPED, WSTOPSIG, WIFCONTINUED
			if(WIFEXITED(status))
			{
				ss << " exited normally with exit status " << WEXITSTATUS(status);
			}
			if(WIFSIGNALED(status))
			{
				ss << " exited by signal " << WTERMSIG(status);

				#ifdef WCOREDUMP
				if(WCOREDUMP(status)) // not available on all unix implementations
				{
					ss << " and had a core dump";
				}
				#endif
			}
			ss.flush();
		}
		outstream << ss.str() << std::endl;
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

template <class layer_t>
void forkLayer(const std::string &process_name, char **argv)
{
	const pid_t pid = fork();
	if(pid == -1)
	{
		auto error = errno;
		std::cerr << "Could not fork" << std::endl;
		switch(error)
		{
			case EAGAIN:
				std::cout << "Thread limit reached or SCHED_DEADLINE used" << std::endl;
				break;
			case ENOMEM:
				std::cout << "Out of memory" << std::endl;
				break;
		}
	}
	else if(pid == 0)
	{
		change_process_name(argv, process_name);
		std::shared_ptr<layer_t> pLayer = std::make_shared<layer_t>();
		pApp->addLayer(pLayer);

		// register shutdown handler
		signal
		(
			SIGTERM,
			[&](int){pApp->stop();}
		);
		signal
		(
			SIGSEGV,
			[&](int)
			{
				pApp->panic();
				std::cout << "framework level segfault detected" << std::endl;

				// remove segfault signal handler
				signal(SIGSEGV, SIG_DFL);

				// force segfault now that it is handled in this process. This
				// will signal the monitor procss that a segfault occured here
				kill(getpid(), SIGSEGV);

				// alternatively, could exit like this:
				// exit(SOME_SEGFAULT_ENUM);
				// then, the monitor would have to respond to that exit value
			}
		);

		pApp->run();
		exit(EXIT_SUCCESS);
	}
	else
	{
		child_datas.emplace
		(
			std::piecewise_construct,
			std::forward_as_tuple(pid),
			std::forward_as_tuple(process_name, pid)
		);
	}
}
