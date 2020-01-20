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
#include "dls2/application_framework/app_layer.hpp"

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

	/// User feedback subscriber
	///
	/// Messages may be published to the framework using the logstream classes.
	/// The `coutstream` class is used for messages that should be directly
	/// displayed to the user. This class listens to those messages and prints
	/// them to the console
	class StringListener : public SubscriberBase<StringMsgPubSubType>
	{
	public:
		/// Constructor
		///
		StringListener(ConsoleLayer &owner);

		/// Destructor
		///
		~StringListener();
	private:
		/// Subscription callback
		///
		/// When a message is received, it is merely put in a buffer of messgaes
		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;

		/// Reference to the console owning this subscriber
		///
		ConsoleLayer &owner;

		// BEGIN critical section
			std::mutex message_stack_mutex;

			/// Condition variable to indicate that there are messages available
			///
			std::condition_variable message_stack_ready;

			/// Messages
			///
			std::vector<std::string> message_stack;

			/// Variable used to tell the print thread to quit
			///
			bool should_quit;
		// END critical section

		/// Thread for printing messages to the console
		///
		std::thread print_thread;

		/// Prints messages to the console
		///
		/// It is possible to print messages directly in the subscriber
		/// callback. This means, however, that each message will be printed
		/// individually and, after each message, the console prompt will be
		/// reprinted. However, it happens sometimes that an event will trigger
		/// multiple messages in quick succession. Printing them directly makes
		/// the console messy and hard to read. Instead, this function collects
		/// a group of messages to be printed together
		void aggregatePrint();
	}string_listener;

	/// Stores commands registered by the console
	///
	CommandManager command_manager;
	std::atomic_bool should_quit;
};
} // end namespace dls

#endif /* end of include guard: CONSOLE_LAYER_HPP_BXNRZS8Q */
