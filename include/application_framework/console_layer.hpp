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
	/// Utility class for collecting a console command
	///
	struct Command
	{
		/// Constructor
		///
		Command
		(
			const std::string &command_name,
			const std::function<void(const std::vector<std::string>&)> &function,
			const std::string &docstring
			// const std::vector<std::string> &default_completions = {}
		);

		/// The command name in the console
		///
		std::string                                    command_name;

		/// The function that is called when the command is input
		///
		std::function<void(std::vector<std::string>)>  function;

		/// The documentation string of the command
		///
		/// This is displayed when calling the `help` command in the console
		std::string                                    docstring;
		// std::vector<std::string>                       default_completions;
	};

	/// Default Constructor
	///
	ConsoleLayer();
	~ConsoleLayer() = default;

	/// Run the console
	///
	/// This is a blocking call that enters an infinite loop
	Status run() override;

	/// Stop the console
	///
	/// Will cause `run` to stop running
	Status shutdown() override;

	/// Adds a command to the console
	///
	void addCommand(const Command&);

private:
	/// Generates the console prompt
	///
	/// This is currently just a stub function, but it can be expanded apon in
	/// future
	std::string build_prompt();

	// Begin critical section
		std::mutex commands_mutex; ///< Mutex protecting the console commands
		std::map<std::string, Command> commands; ///< The commands registered with the console
	// End critical section

	// void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;

	// class CommandRegistrationListener : public SubscriberBase<CommandRegisterMsgPubSubType>
	// {
	// public:
	// 	CommandRegistrationListener(ConsoleLayer &owner);
	// private:
	// 	void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;
	// 	ConsoleLayer &owner;
	// } command_registration_listener;

	RemoteCommandManager remote_command_manager;

	class StringListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		StringListener(ConsoleLayer &owner);
	private:
		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;
		ConsoleLayer &owner;
	// }string_listener;
	};

};
} // end namespace dls

#endif /* end of include guard: CONSOLE_LAYER_HPP_BXNRZS8Q */
