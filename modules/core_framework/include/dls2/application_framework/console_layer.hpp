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
#ifndef CONSOLE_LAYER_HPP_BXNRZS8Q
#define CONSOLE_LAYER_HPP_BXNRZS8Q
// =============================================================================
// Includes
// =============================================================================
// framework
#include "app_layer.hpp"

// messaging
#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/msg/stringmsgPubSubTypes.h"
#include "dls2/msg/command_registerPubSubTypes.h"
#include "dls2/util/messaging/subscriber_base.hpp"

// commands
#include "dls2/command/remote_command.hpp"
#include "dls2/command/command.hpp"

// stdlib
#include <map>
#include <functional>
#include <mutex>
#include <atomic>
#include <signal.h>

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
/// A console interface into the framework
///
class ConsoleLayer : public AppLayer
{
	// Console completion is handled by readline, which is a C-library.
	// Therefore, need to declare this as a friend
	friend char *command_completion(const char *text, int state);
	friend char **console_completion(const char *text, int start, int end);
	friend char *arg_completion(const char * text, int state);

public:
	/// Default Constructor
	///
	ConsoleLayer();

	/// Default Destructor
	///
	~ConsoleLayer() = default;

	/// Run the console
	///
	/// This is a blocking call that enters an infinite loop
	Status run() override;

	/// Stop the console
	///
	/// Will cause `run` to stop running
	Status shutdown() override;

	/// Print the state of this layer
	///
	/// prints:
	/// * active controllers
	/// * active gait generator
	/// * published torque
	std::string where() override{return "not yet implemented";}

private:
	/// Generates the console prompt
	///
	/// This is currently just a stub function, but it can be expanded apon in
	/// future
	std::string build_prompt();

	/// Calls a command with the given command line arguments
	///
	/// This function automatically converts the string arguments to the correct
	/// types to be pushed to the remote command
	/// @param c a remote command to be called
	/// @param args a vector of string representations of the arguments
	void callCommand(const RemoteCommand &c, const std::vector<std::string> &args);

	RemoteCommandManager remote_command_manager;

	/// User feedback subscriber
	///
	version2::Subscriber<StringMsgPubSubType> consoleFeedback;

	/// Stores commands registered by the console
	///
	CommandManager command_manager;

	/// Flag of the running loop
	/// Exits when set to true
	std::atomic_bool should_quit;
	
	/// Handler to the shutdown SIGINT (ctrl+c) signal
	/// 
	static void handle_signals(int);
};
} // end namespace dls

#endif /* end of include guard: CONSOLE_LAYER_HPP_BXNRZS8Q */
