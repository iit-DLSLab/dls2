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
*******************************************************************************/
#ifndef OPTIONS_CPP
#define OPTIONS_CPP

#include <iostream>

#include <getopt.h>

#include "dls2/core_framework/options.hpp"
#include "dls2/core_framework/version_info.hpp"

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls;

// =============================================================================
// Default Initial Values
// =============================================================================
char **Options::argv                        = nullptr;
int  Options::argc                          = 0;

// layers to launch
bool Options::launch_estimation             = false;
bool Options::launch_hardware               = false;
bool Options::launch_control                = false;
bool Options::launch_console                = false;
bool Options::launch_log                    = false;
bool Options::launch_sim                    = false;

// real robot or simulation mode
bool Options::simulation_mode               = true;

// show the documentation in a browser
bool Options::show_docs                     = false;

std::string Options::robot_name             = "UNSPECIFIED_ROBOT_NAME";
static bool robot_is_specified              = false;

// =============================================================================
// Class Implementation
// =============================================================================
bool Options::parseArgs(int argc, char **argv)
{
	if (argc == 1){
		Options::printUsage();
		return false;
	}

	static struct option long_options[] =
	{
		//long_name    required?           return_short?  short_version
		{"robot",      required_argument,  nullptr,       'r'},
		{"simulation", required_argument,  nullptr,       's'},
		{"hardware",   no_argument,        nullptr,       'H'},
		{"layers",     optional_argument,  nullptr,       'l'},
		{"version",    no_argument,        nullptr,       'v'},
		{"help",       no_argument,        nullptr,       'h'},
		// {"core",       no_argument,        nullptr,       'c'},
		{"docs",       no_argument,        nullptr,       'd'},
		{0,            0,                  0,             0}
	};

	// clear the default layers
	Options::launch_estimation  =  false;
	Options::launch_hardware    =  false;
	Options::launch_control     =  false;
	Options::launch_console     =  false;
	Options::launch_log         =  false;
	Options::launch_sim 		=  false;

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
					Options::robot_name = "hyq";
				}
				else if(std::strcmp(optarg, "hyqreal") == 0)
				{
					Options::robot_name = "hyqreal";
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
                Options::launch_sim = true;
				break;
			}
			case 'H':
			{
				Options::simulation_mode = false;
				break;
			}
			case 'l':
			{
				char * const tokens []
				{
					const_cast<char*>("hardware"),
					const_cast<char*>("control"),
					const_cast<char*>("console"),
					const_cast<char*>("log"),
					const_cast<char*>("estimation"),
					const_cast<char*>("sim"),
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
						std::cout << "valid layers are: ";
						for(long unsigned int i = 0; i < (sizeof(tokens)/8-1) ; i++)
						{
							std::cout << tokens[i] << " ";
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
			default:
			{
				goto invalid_command_line;
			}
		}
	}

	// verify if the options are valid
	if (!Options::validate()) 
		return false;
	
	// Save command line flags
	Options::argc = argc;
	Options::argv = argv;

	return true;

invalid_command_line:
	Options::printUsage();
	exit(EXIT_FAILURE);
	return false;
}

void Options::printUsage()
{
	std::cout << "USAGE: dls "

	"< --robot=<hyq|hyqreal> | -r <hyq|hyqreal> > "
	"< --layers= | -l ...> "
	"[ --simulation= | -s ...] "
	"[ --hardware | -H ] "
	"[ --version | -v ] "
	"[ --help | -h ] "
	// "[ --core | -c ] "
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
	// "| core        | c            | launch in core mode                                 |\n"
	"| docs        | d            | show development documentation                      |\n"
	<< std::endl;
}

bool Options::is_simulation(){
	return simulation_mode;
}

bool Options::validate()
{
	// if show docs is enabled, main will just show the docs and exit
	// Not doing more checks in that case
	if(Options::show_docs)
		return false;

	if
	(
		(Options::launch_control || Options::launch_hardware) &&
		!robot_is_specified
	)
	{
		std::cerr << "Error: robot not specified" << std::endl;
		Options::printUsage();
		exit(EXIT_FAILURE);
		return false;
	}

	if(
		!launch_estimation &&
		!launch_hardware &&
		!launch_control &&
		!launch_console &&
		!launch_log &&
		!launch_sim
	)
		return false;

	return true;
}

#endif // OPTIONS_CPP