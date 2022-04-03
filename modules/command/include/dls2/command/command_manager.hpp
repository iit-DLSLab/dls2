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
#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

// =============================================================================
// Includes
// =============================================================================
#include "dls2/command/command.hpp"
#include <string>
#include <utility>
#include <vector>
#include <condition_variable>

namespace dls
{

// =============================================================================
// Callable
// =============================================================================
/// A class that can be used to programatically call external commands as if
/// they are defined in the same process space
///
class RemoteCommandCallable
{
	friend class CommandManager;
public:
	template <typename... Ts>
	void operator()(Ts...);

private:
	RemoteCommandCallable();
	std::shared_ptr<CommandBase> pRemote_command;
};
// =============================================================================
// Command Manager Class
// =============================================================================
/// A class that manages commands
///
/// This class updates its internal representations as commands advertise
/// themselves and remove themselves from the framework
class CommandManager
{
public:
	/// Constructor
	///
	CommandManager(std::string owner_);

	/// Destructor
	///
	~CommandManager();

	/// Find commands
	///
	/// Finds a vector of commands by the name of the component that owns them
	std::vector<std::shared_ptr<CommandBase>> findByOwner( const std::string& ) const;

	/// Find commands
	///
	/// Finds a vector of commands by their name. Note that certain commands may
	/// have the same name but different owners. This could be the case, for
	/// instance, if multiple controllers advertise a `start` or `stop` command.
	std::vector<std::shared_ptr<CommandBase>> findByName( const std::string& ) const;

	/// Find command
	///
	/// Finds a command by its name and the name of its owner. There is
	/// guaranteed to be at most one command of this type. Returns nullptr on
	/// failure
	std::shared_ptr<CommandBase> find( const std::string &owner, const std::string &name ) const;

	/// Get list of all registered commands
	///
	/// Since commands may be added from separate processes, it is not possible
	/// to give direct access to the CommandManager's list of commands. Instead,
	/// this command makes a copy of the registered commands and returns that.
	/// That way, if some component starts walking through its list of commands,
	/// that list will never get invalidated by another process registering new
	/// commands
	std::vector<std::shared_ptr<CommandBase>> getCurrentlyRegisteredCommands();

	/// Get a list of the unique owners of the commands
	///
	std::set<std::string> getCurrentlyRegisteredOwners();

	/// Give a command the hability to be remote callable
	///
	/// @param owner the name of the component that registers the command
	/// @param name the name of the command registered by that component
	/// @return a functor representing the command
	RemoteCommandCallable makeCallable
	(
		const std::string &owner,
		const std::string &name
	) const;

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
		const std::string &command_name,
		const std::string &docstring,
		const std::function<ret_t(arg_ts...)> &f,
		const bool is_remote = false
	);

	/// Removes a command from the manager
	///
	void removeCommand(CommandBase);

private:
	// begin critical section
		/// Mutex protecting the `remote_commands` vector
		///
		mutable std::mutex commands_mutex;

		/// Storage space for the commands
		///
		std::vector<std::shared_ptr<CommandBase>> commands;

		/// Used by makeCallable to check whether a command that had not been
		/// registered has become available
		///
		mutable std::condition_variable command_added;
	// end critical section

	/// Owner layer of the commands
	///
	std::string owner;

	/// Subscriber to receive informations from the command dss domain
	///
	version2::Subscriber<CommandRegisterMsgPubSubType> registration_listener;

};

} // end namespace dls

#include "dls2/command/command_manager.tpp"

#endif /* end of include guard: COMMAND_MANAGER_HPP */
