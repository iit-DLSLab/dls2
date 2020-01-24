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
#include <getopt.h>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <cstring>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include "dls2/application_framework/hyq_app.hpp"
#include "dls2/application_framework/hardware_layer.hpp"
#include "dls2/application_framework/control_layer.hpp"
#include "dls2/application_framework/estimation_layer.hpp"
#include "dls2/application_framework/log_layer.hpp"
#include "dls2/application_framework/console_layer.hpp"

#include "dls2/util/debug/debug.hpp"
#include "dls2/util/log/log.hpp"
#include "dls2/command/command.hpp"

#include <thread>

#include "dls2/geometry/pose.hpp"

#include "dls2/util/log/log.hpp"

#include <doglib/factory/robot_factory.hpp>

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

/// Layers
///
enum class layer_decl
{
	CONTROL,
	CONSOLE,
	ESTIMATION,
	HARDWARE,
	LOG
};

/// Command line options struct
///
struct options
{
	options() :
		robot(dls::dog::RobotFactory::RobotType::HyQ),
		simulation_nhardware(true),
		layers
		{
			layer_decl::CONTROL,
			layer_decl::CONSOLE
		}
	{ }
	dls::dog::RobotFactory::RobotType robot;
	bool simulation_nhardware;
	std::vector<layer_decl> layers;
};


// =============================================================================
// Prtototypes
// =============================================================================
void handle_args(int argc, char **argv, options*);
void print_usage(int argc, char **argv);

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
	options opt;
	handle_args(argc, argv, &opt);

	// Ignore cntrl-C
	//
	// To exit the framework, an exit command is registered later
	signal(SIGINT, SIG_IGN);

	// Create application
	pApp = std::make_shared<HyQApp>();

	// Launch layers
	for(const auto &el : opt.layers)
	{
		if(el == layer_decl::LOG)
		{
			forkLayer<LogLayer>("log_layer", argv);
		}
		else if(el == layer_decl::CONSOLE)
		{
			forkLayer<ConsoleLayer>("console_layer", argv);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else if(el == layer_decl::HARDWARE)
		{
			forkLayer<HardwareLayer>("hardware_layer", argv);
		}
		else if(el == layer_decl::CONTROL)
		{
			forkLayer<ControlLayer>("control_layer", argv);
		}
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

	command_manager.addCommand<void, std::string>
	(
		"HyQApp_server",
		"launchLayer",

		"Launches a layer specified by its argument:\n"
		"* hardware\n"
		"* console\n"
		"* control\n"
		"* log",

		std::function<void(std::string)>
		(
			[&](std::string s)
			{
				if(s == "hardware") forkLayer<HardwareLayer>("hardware_layer", argv);
				else if(s == "console") forkLayer<ConsoleLayer>("console_layer", argv);
				else if(s == "control") forkLayer<ControlLayer>("control_layer", argv);
				else if(s == "log") forkLayer<LogLayer>("log_layer", argv);
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

void handle_args(int argc, char **argv, options *opts)
{
	static struct option long_options[] =
	{
		//long_name    required?           return_short?  short_version
		{"robot",      required_argument,  nullptr,       'r'},
		{"simulation", no_argument,        nullptr,       's'},
		{"hardware",   no_argument,        nullptr,       'H'},
		{"layers",     optional_argument,  nullptr,       'l'},
		{"version",    no_argument,        nullptr,       'v'},
		{"help",       no_argument,        nullptr,       'h'},
		{0,            0,                  0,             0}
	};

	int opt;
	bool robot_specified = false;
	while((opt = getopt_long(argc, argv, "r:sHl:vh::", long_options, nullptr)) != -1)
	{
		switch(opt)
		{
			case 'r':
			{
				if(std::strcmp(optarg, "hyq") == 0)
				{
					opts->robot = dls::dog::RobotFactory::RobotType::HyQ;
				}
				else if(std::strcmp(optarg, "hyqreal") == 0)
				{
					opts->robot = dls::dog::RobotFactory::RobotType::HyQReal;
				}
				else
				{
					std::cerr << "Unknown robot: " << optarg << std::endl;
					goto invalid_command_line;
				}
				robot_specified = true;
				break;
			}
			case 's':
			{
				opts->simulation_nhardware = true;
				break;
			}
			case 'H':
			{
				opts->simulation_nhardware = false;
				break;
			}
			case 'l':
			{
				opts->layers.clear();
				char * const tokens []
				{
					const_cast<char*>("hardware"),
					const_cast<char*>("control"),
					const_cast<char*>("console"),
					const_cast<char*>("log"),
					const_cast<char*>("estimation"),
					nullptr
				};
				char *value;
				int opt;
				char *subopts = optarg;

				while(*subopts != '\0')
				{
					opt = getsubopt(&subopts, tokens, &value);
					if(opt != -1)
					{
						auto layer = tokens[opt];
						if(std::strcmp(layer, "hardware") == 0)
						{
							opts->layers.push_back(layer_decl::HARDWARE);
						}
						else if(std::strcmp(layer, "control") == 0)
						{
							opts->layers.push_back(layer_decl::CONTROL);
						}
						else if(std::strcmp(layer, "console") == 0)
						{
							opts->layers.push_back(layer_decl::CONSOLE);
						}
						else if(std::strcmp(layer, "log") == 0)
						{
							opts->layers.push_back(layer_decl::LOG);
						}
						else if(std::strcmp(layer, "estimation") == 0)
						{
							opts->layers.push_back(layer_decl::ESTIMATION);
						}
					}
					else
					{
						std::cerr << "unknown layer: " << value << std::endl;
						goto invalid_command_line;
					}
				}
				break;
			}
			case 'v':
			{
				std::cout << "asked for version" << std::endl;
				break;
			}
			case 'h':
			{
				// TODO print help of specific arg
				print_usage(argc, argv);
				exit(EXIT_SUCCESS);
				break;
			}
			default:
			{
				goto invalid_command_line;
			}
		}
	}

	if(!robot_specified)
	{
		std::cerr << "Error: robot not specified" << std::endl;
		goto invalid_command_line;
	}

	return;

invalid_command_line:
	print_usage(argc, argv);
	exit(EXIT_FAILURE);
}

void print_usage(int /*argc*/, char **argv)
{
	std::cout << "USAGE: " << argv[0] << " "

	"< -r <hyq|hyqreal> | --robot=<hyq|hyqreal> > "
	"< --layers= | l ...> "
	"[ --simulation | -s] "
	"[ --hardware | -H ] "
	"[ --version | -v ] "
	"[ --help - h ]"

	"\n"
	"\n"
	"Flag meanings:\n"
	"==============\n"
	"| long option | short option | meaning                                             |\n"
	"|-------------|--------------|-----------------------------------------------------|\n"
	"| robot       | r            | specify the robot on which the framework is running |\n"
	"| simulation  | s            | run the framework in simulated mode (default)       |\n"
	"| hardware    | H            | run the framework on the real robot                 |\n"
	"| layers      | l            | a comma-separated list of layers to launch          |\n"
	"| version     | v            | print the version and exit                          |\n"
	"| help        | h            | print this help and exit                            |\n"
	<< std::endl;
}
