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
#ifndef REMOTE_COMMAND_HPP_EDSRALCP
#define REMOTE_COMMAND_HPP_EDSRALCP

// =============================================================================
// Includes
// =============================================================================
#include <condition_variable>
#include <functional>
#include <memory>
#include <memory>
#include <set>

#include <fastrtps/publisher/PublisherListener.h>
#include <fastrtps/types/DynamicPubSubType.h>
#include <fastrtps/participant/Participant.h>

#include "dls2/command/command_base.hpp"
#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/util/messaging/subscriber_base.hpp"
#include "dls2/domains/domains.hpp"

namespace dls
{
// =============================================================================
// Forward Declarations
// =============================================================================
class RemoteCommandManager;
// =============================================================================
// Class Interface
// =============================================================================
/// Class representing an external command
///
/// An external command is registered via an instance of CommandManager by
/// another component in the architecture
class RemoteCommand : public CommandBase
{
	friend class RemoteCommandManager;
public:
	/// Constructor
	///
	RemoteCommand(const std::string &topic_, CommandRegisterMsg &msg);

	template <typename T>
	void pushArg(T t) const;

	template <typename U, typename... Ts>
	void pushArg(U u, Ts... ts) const;

	void call() const;

	/// The name of the component that owns the remote command
	///
	const std::string owner;

	/// The name of the command
	///
	const std::string command_name;

	/// Command documentation
	///
	const std::string docstring;

	/// Argument types
	///
	/// The type of arguments to this command are represented in a vector
	const std::remove_reference<CommandBase::RepresentationVector>::type args;
	// const std::vector<unsigned long long> args;

	/// The return type of this command
	///
	const std::remove_reference
	<
		CommandBase::RepresentationVector
	>::type::value_type
	ret_type;

private:

	// Publisher to call remote commands
	dls::version2::Publisher<CommandRegisterMsgPubSubType> remote_command_publisher;

};

// =============================================================================
// Callable
// =============================================================================
/// A class that can be used to programatically call external commands as if
/// they are defined in the same process space
///
class RemoteCommandCallable
{
	friend class RemoteCommandManager;
public:
	template <typename... Ts>
	void operator()(Ts...);

private:
	RemoteCommandCallable();
	std::shared_ptr<const RemoteCommand> pRemote_command;
};
// =============================================================================
// Manager Class
// =============================================================================
/// A class that manages remote commands
///
/// This class updates its internal representations as remote commands advertise
/// themselves and remove themselves from the framework
class RemoteCommandManager
{
public:
	/// Constructor
	///
	/// @param onNewCommand an optional callback when a new command is
	/// registered in the framework
	/// @param onRemoveCommand an optional callback when a command is removed
	/// from the framework
	RemoteCommandManager
	(
		std::function<void(std::shared_ptr<const RemoteCommand>)>
			onNewCommand = nullptr,
		std::function<void(std::shared_ptr<const RemoteCommand>)>
			onRemoveCommand = nullptr
	);

	/// Find commands
	///
	/// Finds a vector of commands by the name of the component that owns them
	std::vector<std::shared_ptr<const RemoteCommand>> findByOwner
	(
		const std::string&
	) const;

	/// Find commands
	///
	/// Finds a vector of commands by their name. Note that certain commands may
	/// have the same name but different owners. This could be the case, for
	/// instance, if multiple controllers advertise a `start` or `stop` command.
	std::vector<std::shared_ptr<const RemoteCommand>> findByName
	(
		const std::string&
	) const;

	/// Find command
	///
	/// Finds a command by its name and the name of its owner. There is
	/// guaranteed to be at most one command of this type. Returns nullptr on
	/// failure
	std::shared_ptr<const RemoteCommand> find
	(
		const std::string &owner, const std::string &name
	) const;

	/// Get list of all registered commands
	///
	/// Since commands may be added from separate processes, it is not possible
	/// to give direct access to the CommandManager's list of commands. Instead,
	/// this command makes a copy of the registered commands and returns that.
	/// That way, if some component starts walking through its list of commands,
	/// that list will never get invalidated by another process registering new
	/// commands
	std::vector<std::shared_ptr<const RemoteCommand>>
		getCurrentlyRegisteredCommands();

	/// Get a list of the unique owners of the commands
	///
	std::set<std::string> getCurrentlyRegisteredOwners();

	/// Generates a RemoteCommandCallable for a given command
	///
	/// @param owner the name of the component that registers the command
	/// @param name the name of the command registered by that component
	/// @return a functor representing the command
	RemoteCommandCallable makeCallable
	(
		const std::string &owner,
		const std::string &name
	) const;

	void onNewDataMessage(eprosima::fastrtps::Subscriber *sub);

private:
	// begin critical section
		/// Mutex protecting the `remote_commands` vector
		///
		mutable std::mutex remote_commands_mutex;

		/// The internal representation of remote commands
		///
		std::vector<std::shared_ptr<const RemoteCommand>> remote_commands;

		/// Used by makeCallable to check whether a command that had not been
		/// registered has become available
		///
		mutable std::condition_variable command_added;
	// end critical section

	/// Subscriber to receive informations from the command dss domain
	///
	version2::Subscriber<CommandRegisterMsgPubSubType> registration_listener;

	/// Callback when a command is added to the framework
	///
	std::function<void(std::shared_ptr<const RemoteCommand>)> onNewCommand;

	/// Callback when a command is removed from the framework
	///
	std::function<void(std::shared_ptr<const RemoteCommand>)> onRemoveCommand;

	/// Adds a command to the manager
	///
	void addCommand(std::shared_ptr<RemoteCommand>);

	/// Removes a command from the manager
	///
	void removeCommand(const CommandRegisterMsg &msg);
};
} // end namespace dls

#include "dls2/command/remote_command.tpp"
#endif /* end of include guard: REMOTE_COMMAND_HPP_EDSRALCP */
