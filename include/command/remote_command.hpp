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
#ifndef REMOTE_COMMAND_HPP_EDSRALCP
#define REMOTE_COMMAND_HPP_EDSRALCP

// =============================================================================
// Includes
// =============================================================================
#include <memory>
#include <functional>
#include "command/command_base.hpp"
#include "util/messaging/subscriber_base.hpp"

#include <fastrtps/types/DynamicPubSubType.h>
#include <fastrtps/participant/Participant.h>
#include <fastrtps/publisher/PublisherListener.h>
#include <memory>

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
	RemoteCommand(CommandRegisterMsg &msg);

	template <typename T>
	void pushArg(T t) const;
	void call() const;

private:
	void clearArgs() const;

// private:
// TODO make private again
public:
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
	const std::remove_reference<CommandBase::RepresentationVector>::type::value_type ret_type;

private:
	/// Dynamic publisher helper class
	///
	/// Note that when a remote command is created, there will be a delay of
	/// around 80ms (determined experimentally) before which it can be used. If
	/// the command is called within this period, then it may be missed by the
	/// rest of the framework
	class RemoteCommandPublisher : public eprosima::fastrtps::PublisherListener
	{
	public:
		RemoteCommandPublisher(RemoteCommand &owner, const CommandRegisterMsg &msg);

		eprosima::fastrtps::types::DynamicPubSubType dynamic_type;
		std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData;
		std::shared_ptr<eprosima::fastrtps::Participant> pParticipant;
		std::shared_ptr<eprosima::fastrtps::Publisher> pPublisher;
		// RemoteCommand &owner:

		/// The index of the next command to be pushed into the dynamic data
		/// type pData
		mutable size_t command_arg_index;
	}remote_command_publisher;

	void requestRegistration() override {}
	void requestDeregistration() override {}
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
		std::function<void(std::shared_ptr<const RemoteCommand>)> onNewCommand = nullptr,
		std::function<void(std::shared_ptr<const RemoteCommand>)> onRemoveCommand = nullptr
	);

	/// Find commands
	///
	/// Finds a vector of commands by the name of the component that owns them
	std::vector<std::shared_ptr<const RemoteCommand>> findByOwner(const std::string&) const;

	/// Find commands
	///
	/// Finds a vector of commands by their name. Note that certain commands may
	/// have the same name but different owners. This could be the case, for
	/// instance, if multiple controllers advertise a `start` or `stop` command.
	std::vector<std::shared_ptr<const RemoteCommand>> findByName(const std::string&) const;

	/// Find command
	///
	/// Finds a command by its name and the name of its owner. There is
	/// guaranteed to be at most one command of this type. Returns nullptr on
	/// failure
	std::shared_ptr<const RemoteCommand> find(const std::string &owner, const std::string &name) const;

	/// Get list of all registered commands
	///
	/// Since commands may be added from separate processes, it is not possible
	/// to give direct access to the CommandManager's list of commands. Instead,
	/// this command makes a copy of the registered commands and returns that.
	/// That way, if some component starts walking through its list of commands,
	/// that list will never get invalidated by another process registering new
	/// commands
	std::vector<std::shared_ptr<const RemoteCommand>> getCurrentlyRegisteredCommands();

private:
	// begin critical section
		/// Mutex protecting the `remote_commands` vector
		///
		mutable std::mutex remote_commands_mutex;

		/// The internal representation of remote commands
		///
		std::vector<std::shared_ptr<const RemoteCommand>> remote_commands;
	// end critical section

	/// Helper Listener class
	///
	/// This class listens for remote commands to register themselves and
	/// informs its owner RemoteCommandManager about their presence
	class RegistrationListener : public SubscriberBase<CommandRegisterMsgPubSubType>
	{
	public:
		/// Constructor
		///
		RegistrationListener(RemoteCommandManager &owner);
	private:

		/// Subscriber Callback
		///
		void onNewDataMessage(eprosima::fastrtps::Subscriber*) override;

		/// Owner instance
		///
		RemoteCommandManager &owner;
	} registration_listener;

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
#endif /* end of include guard: REMOTE_COMMAND_HPP_EDSRALCP */
