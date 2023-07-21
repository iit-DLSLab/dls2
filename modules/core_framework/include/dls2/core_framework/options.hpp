
#ifndef LAUNCH_OPTIONS_HPP_HNQYDBQW
#define LAUNCH_OPTIONS_HPP_HNQYDBQW
// =============================================================================
// Includes
// =============================================================================
// stdlib
#include <string>
#include <map>

// libs
#include "robotlib/robot_factory.hpp"

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
	/// Parses the arguments and updates the representations
	/// Initialises the options static object
	///
	/// This must be called on startup of the framework with the arguments
	/// passed to main. This will then build the internal representation of the
	/// options for ease of use from the rest of the framework
	/// Return if the args are valid and if there is anything to do other than printing usage
	static bool parseArgs(int argc, char **argv);

	/// Prints the command-line usage of the framework
	static void printUsage();

	/// The raw argv and argc as passed to main
	static char **argv;
	static int argc;

	// Layers to launch
	static bool launch_estimation;
	static bool launch_hardware;
	static bool launch_control;
	static bool launch_console;
	static bool launch_log;
	static bool launch_service;
	static std::string estimation_layer_name;

	static std::string robot_name;

	// launch browser for documentation
	static bool show_docs;

private:
	/// Validates whether the command line arguments are correct. Exits the
	/// program if they are not.
	static bool validate();
};

} // end namespace dls

#endif /* end of include guard: LAUNCH_OPTIONS_HPP_HNQYDBQW */
