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
#ifndef LAUNCH_OPTIONS_HPP_HNQYDBQW
#define LAUNCH_OPTIONS_HPP_HNQYDBQW
// =============================================================================
// Includes
// =============================================================================
// stdlib
#include <string>
#include <map>

// libs
#include <doglib/factory/robot_factory.hpp>

namespace dls
{

// =============================================================================
// Class Interface
// =============================================================================
/// This class is a simple interface into the command-line options that the
/// framework was launched with
///
/// This is a static class. Any component can access its information
class Options
{
public:
	/// Initialises the options static object
	///
	/// This must be called on startup of the framework with the arguments
	/// passed to main. This will then build the internal representation of the
	/// options for ease of use from the rest of the framework
	static void init(int argc,  char **argv);

	/// Prints the command-line usage of the framework
	///
	static void printUsage();

	/// The raw argv as passed to main
	///
	static char **argv;

	/// The raw argc as passed to main
	///
	static int argc;

	// Layers to launch
	static bool launch_estimation;
	static bool launch_hardware;
	static bool launch_control;
	static bool launch_console;
	static bool launch_log;

	// Real robot or simulation mode
	static bool simulation_mode;

	// if this is the core
	static bool is_core;

	static dls::dog::RobotFactory::RobotType robot;

	// launch browser for documentation
	static bool show_docs;

private:
	/// Parses the arguments and updates the representations
	///
	static void parseArgs(int argc, char **argv);

	/// Validates whether the command line arguments are correct. Exits the
	/// program if they are not.
	static void validate();
};

} // end namespace dls

#endif /* end of include guard: LAUNCH_OPTIONS_HPP_HNQYDBQW */
