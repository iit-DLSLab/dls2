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
#ifndef CONSOLE_LAYER_HPP_BXNRZS8Q
#define CONSOLE_LAYER_HPP_BXNRZS8Q
// =============================================================================
// Includes
// =============================================================================
// framework
#include "application_framework/app_layer.hpp"

// messaging
#include "util/messaging/publisher_base.hpp"
#include "msg/stringmsgPubSubTypes.h"
#include "msg/command_registerPubSubTypes.h"
#include "util/messaging/subscriber_base.hpp"

// commands
#include "command/remote_command.hpp"
#include "command/command.hpp"

// stdlib
#include <map>
#include <functional>
#include <mutex>

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
/// A console interface into the framework
///
class ConsoleLayer : public AppLayer//, public SubscriberBase<StringMsgPubSubType>
{
	// Console completion is handled by readline, which is a C-library.
	// Therefore, need to declare this as a friend
	friend char *dls::command_completion(const char *text, int state);
	friend char **dls::console_completion(const char *text, int start, int end);
	friend char *dls::arg_completion(const char * text, int state);

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

	class StringListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		StringListener(ConsoleLayer &owner);
	private:
		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;
		ConsoleLayer &owner;
	}string_listener;
	// };

	CommandManager command_manager;

	//// Begin Critical section
	//	/// Command container
	//	///
	//	/// Since commands are dynamically added by different processes
	//	/// potentially writen by different developers, there may be commands
	//	/// that have name clashes. Therefore, each command is first indexed by
	//	/// the name of the component that owns the command, then by the name of
	//	/// the command
	//	std::map
	//	<
	//		std::string,								// command owner name
	//		std::map
	//		<
	//			std::string,							// command nmae
	//			std::shared_ptr<const RemoteCommand>	// command
	//		>
	//	> commands;

	//	/// Mutex protects `commands`
	//	///
	//	std::mutex commands_mutex;
	//// END Critical section

};
} // end namespace dls

#endif /* end of include guard: CONSOLE_LAYER_HPP_BXNRZS8Q */
