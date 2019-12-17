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

namespace dls
{
// =============================================================================
// Forward Declarations
// =============================================================================
class RemoteCommandManager;
// =============================================================================
// Class Interface
// =============================================================================
class RemoteCommand : public CommandBase
{
	friend class RemoteCommandManager;
public:
	RemoteCommand(CommandRegisterMsg &msg);

private:
	void clearArgs();
	template <typename T>
	void pushArg(T t);
	void call();

private:
	const std::string owner;
	const std::string command_name;
	const std::string docstring;
	const CommandBase::RepresentationVector args;
	const std::remove_reference<CommandBase::RepresentationVector>::type::value_type ret_type;
};
// =============================================================================
// Manager Class
// =============================================================================
class RemoteCommandManager
{
public:
	RemoteCommandManager
	(
		std::function<void(std::shared_ptr<RemoteCommand>)> onNewCommand = nullptr,
		std::function<void(std::shared_ptr<RemoteCommand>)> onRemoveCommand = nullptr
	);

	std::vector<std::shared_ptr<RemoteCommand>> findByOwner(const std::string&);
	std::vector<std::shared_ptr<RemoteCommand>> findByName(const std::string&);
	std::shared_ptr<RemoteCommand> find(const std::string &owner, const std::string &name);

private:
	// begin critical section
		std::mutex remote_commands_mutex;
		std::vector<std::shared_ptr<RemoteCommand>> remote_commands;
	// end critical section

	class RegistrationListener : public SubscriberBase<CommandRegisterMsgPubSubType>
	{
	public:
		RegistrationListener(RemoteCommandManager &owner);
	private:
		void onNewDataMessage(eprosima::fastrtps::Subscriber*) override;
		RemoteCommandManager &owner;
	} registration_listener;

	std::function<void(std::shared_ptr<RemoteCommand>)> onNewCommand;
	std::function<void(std::shared_ptr<RemoteCommand>)> onRemoveCommand;
};
} // end namespace dls

#include "command/remote_command.tpp"
#endif /* end of include guard: REMOTE_COMMAND_HPP_EDSRALCP */
