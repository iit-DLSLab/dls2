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
// stdlib
#include <iostream>

// libs
#include <getopt.h>

// project includes
#include "dls2/application_framework/options.hpp"
#include "dls2/application_framework/version_info.hpp"

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls;

// =============================================================================
// Default Initial Values
// =============================================================================
char  **Options::argv  =  nullptr;
int   Options::argc    =  0;

// layers to launch
bool  Options::launch_estimation  =      true;
bool  Options::launch_hardware    =      false;
bool  Options::launch_control     =      true;
bool  Options::launch_console     =      true;
bool  Options::launch_log         =      false;

// real robot or simulation mode
bool  Options::simulation_mode    =      true;

// if this is the core
bool  Options::is_core            =      false;

// show the documentation in a browser
bool Options::show_docs = false;

dog::RobotFactory::RobotType Options::robot = dog::RobotFactory::RobotType::HyQ;
static bool robot_is_specified = false;

// =============================================================================
// Class Implementation
// =============================================================================
void Options::init(int argc, char **argv)
{
	// Save command line flags
	Options::argc = argc;
	Options::argv = argv;

	Options::parseArgs(argc, argv);
	Options::validate();
}

void Options::parseArgs(int argc, char **argv)
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
		{"core",       no_argument,        nullptr,       'c'},
		{"docs",       no_argument,        nullptr,       'd'},
		{0,            0,                  0,             0}
	};

	int opt;
	while((opt = getopt_long(argc, argv, "r:sHl:vh::cd", long_options, nullptr)) != -1)
	{
		switch(opt)
		{
			case 'd':
			{
				Options::show_docs = true;
				break;
			}
			case 'r':
			{
				robot_is_specified = true;
				if(std::strcmp(optarg, "hyq") == 0)
				{
					Options::robot = dls::dog::RobotFactory::RobotType::HyQ;
				}
				else if(std::strcmp(optarg, "hyqreal") == 0)
				{
					Options::robot = dls::dog::RobotFactory::RobotType::HyQReal;
				}
				else
				{
					std::cerr << "Unknown robot: " << optarg
						<<"\nvalid options are 'hyq' or 'hyqreal'" << std::endl;
					goto invalid_command_line;
				}
				break;
			}
			case 's':
			{
				Options::simulation_mode = true;
				break;
			}
			case 'H':
			{
				Options::simulation_mode = false;
				break;
			}
			case 'l':
			{
				// clear the default layers
				Options::launch_estimation  =  false;
				Options::launch_hardware    =  false;
				Options::launch_control     =  false;
				Options::launch_console     =  false;
				Options::launch_log         =  false;

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
							Options::launch_hardware = true;
						}
						else if(std::strcmp(layer, "control") == 0)
						{
							Options::launch_control = true;
						}
						else if(std::strcmp(layer, "console") == 0)
						{
							Options::launch_console = true;
						}
						else if(std::strcmp(layer, "log") == 0)
						{
							Options::launch_log = true;
						}
						else if(std::strcmp(layer, "estimation") == 0)
						{
							Options::launch_estimation = true;
						}
					}
					else
					{
						std::cerr << "unknown layer: " << value << std::endl;
						auto p = tokens;
						std::cout << "valid layers are: ";
						while(p != nullptr)
						{
							std::cout << p << " ";
						}
						std::cout << std::endl;
						goto invalid_command_line;
					}
				}
				break;
			}
			case 'v':
			{
				std::cout << "Version: " << VersionInfo::getVersionString() << std::endl;
				exit(EXIT_SUCCESS);
				break;
			}
			case 'h':
			{
				// TODO print help of specific arg
				Options::printUsage();
				exit(EXIT_SUCCESS);
				break;
			}
			case 'c':
			{
				Options::is_core = true;
				break;
			}
			default:
			{
				goto invalid_command_line;
			}
		}
	}

	return;

invalid_command_line:
	Options::printUsage();
	exit(EXIT_FAILURE);
}

void Options::printUsage()
{
	std::cout << "USAGE: " << Options::argv[0] << " "

	"< -r <hyq|hyqreal> | --robot=<hyq|hyqreal> > "
	"< --layers= | l ...> "
	"[ --simulation | -s] "
	"[ --hardware | -H ] "
	"[ --version | -v ] "
	"[ --help | -h ] "
	"[ --core | -c ] "
	"[ --doc | -d ]"
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
	"| core        | c            | launch in core mode                                 |\n"
	"| doc         | d            | show development documentation                      |\n"
	<< std::endl;
}

void Options::validate()
{
	if(Options::show_docs)
	{
		// if show docs is enabled, main will just show the docs and exit
		// Not doing more checks in that case
		return;
	}
	if
	(
		(Options::launch_control || Options::launch_hardware) &&
		!robot_is_specified
	)
	{
		std::cerr << "Error: robot not specified" << std::endl;
		Options::printUsage();
		exit(EXIT_FAILURE);
	}
}
