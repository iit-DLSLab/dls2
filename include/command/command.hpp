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
#ifndef COMMAND_HPP_RSTXNA3I
#define COMMAND_HPP_RSTXNA3I

// =============================================================================
// Includes
// =============================================================================
#include "command/command_base.hpp"
#include "util/messaging/publisher_base.hpp"
#include "util/messaging/subscriber_base.hpp"
#include "msg/command_registerPubSubTypes.h"
#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <memory>

namespace dls
{
// =============================================================================
// Class Interface
// =============================================================================
/// Template class representing a local command
///
/// This class is used to register a command with the framework. Users should
/// prefer using CommandManager instead of using Command directly
template <typename ret_t, typename...arg_ts>
class Command : public CommandBase
{
public:
	/// Constructor
	///
	/// @param owner the component that registered this command
	/// @param command_name the name of the command as seen from the rest of the
	/// framework
	/// @param docstring some documentation for the command
	/// @param f the function associated with the command. The function may take
	/// any number of any type of argument, and returns any type
	Command
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring,
		const std::function<ret_t(arg_ts...)> &f
	);

	/// Registers thi command with the framework
	///
	void requestRegistration();

	/// Removes this command from the framework
	///
	void requestDeregistration();

private:
	// ========================== Constructor helpers ==========================
	/// Constructor helper
	///
	/// Builds the CommandRegisterMsg used to advertise this command
	CommandRegisterMsg buildMsg
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring
	);

	// ============================= Data Members ==============================
	/// Name of the component that owns this command
	///
	const std::string owner;

	/// Name of the command as seen by the rest of the framework
	///
	const std::string command_name;

	/// Documentation for the command
	///
	const std::string docstring;

	/// Callback of the command
	///
	const std::function<ret_t(arg_ts...)> f;

	/// Registration message of the command
	///
	CommandRegisterMsg msg;

	/// Publisher used to register the command with the rest of the framework
	///
	PublisherBase<CommandRegisterMsgPubSubType> publisher;
};

// =============================================================================
// Container Class
// =============================================================================
/// Command Manager class
///
/// External components should prefer using this class instead of directly using
/// `Command`
class CommandManager
{
public:
	/// Constructor
	///
	CommandManager();

	/// Adds a command to the CommandManager and registers it with the rest of
	/// the framework
	///
	/// @param owner name of the component that owns the command
	/// @command_name command name as seen by the rest of the framework
	/// @param docstring some documentation for the command
	/// @param f the function encapsulated by the command
	template <typename ret_t, typename... arg_ts>
	void addCommand
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring,
		const std::function<ret_t(arg_ts...)> &f
	);

private:
	/// Storage space for the commands
	///
	std::vector<std::unique_ptr<CommandBase>> commands;
};
} // end namespace dls

#include "command/command.tpp"

#endif /* end of include guard: COMMAND_HPP_RSTXNA3I */
