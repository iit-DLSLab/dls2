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
// =============================================================================
// Includes
// =============================================================================
#include "command/remote_command.hpp"
#include "topics/command_register.hpp"
#include <vector>

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls;

// =============================================================================
// Remote Command
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
RemoteCommand::RemoteCommand(CommandRegisterMsg &msg) :
	owner(msg.owner()),
	command_name(msg.command_name()),
	docstring(msg.docstring()),
	args(msg.arg_types()),
	ret_type(msg.ret_type())
{ }

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
void RemoteCommand::clearArgs()
{
	// TODO implement
}

void RemoteCommand::call()
{
	// TODO implement
}

// =============================================================================
// Remote Command Manager
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
RemoteCommandManager::RemoteCommandManager
(
	std::function<void(std::shared_ptr<RemoteCommand>)> onNewCommand_,
	std::function<void(std::shared_ptr<RemoteCommand>)> onRemoveCommand_
) :
	remote_commands_mutex(),
	remote_commands(),
	registration_listener(*this),
	onNewCommand(onNewCommand_),
	onRemoveCommand(onRemoveCommand_)
{ }

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
std::vector<std::shared_ptr<RemoteCommand>> RemoteCommandManager::findByOwner
(
	const std::string &owner
)
{
	std::vector<std::shared_ptr<RemoteCommand>> vec;
	{
		std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
		for(const auto &el : this->remote_commands)
		{
			if(el->owner == owner)
			{
				vec.push_back(el);
			}
		}
	}
	return vec;
}

std::vector<std::shared_ptr<RemoteCommand>> RemoteCommandManager::findByName
(
	const std::string &name
)
{
	std::vector<std::shared_ptr<RemoteCommand>> vec;
	{
		std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
		for(const auto &el : this->remote_commands)
		{
			if(el->command_name == name)
			{
				vec.push_back(el);
			}
		}
	}
	return vec;
}

std::shared_ptr<RemoteCommand> RemoteCommandManager::find
(
	const std::string &owner,
	const std::string &name
)
{
	{
		std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
		for(const auto &el : this->remote_commands)
		{
			if
			(
				el->owner == owner &&
				el->command_name == name
			)
			{
				return el;
			}
		}
	}
	return nullptr;
}

// -----------------------------------------------------------------------------
// Subscriber Helper
// -----------------------------------------------------------------------------
// =============================== Constructors ================================
RemoteCommandManager::RegistrationListener::RegistrationListener
(
	RemoteCommandManager &owner_
) :
	SubscriberBase<CommandRegisterMsgPubSubType>(topics::command_register),
	owner(owner_)
{ }

// ============================== Implementation ===============================
void RemoteCommandManager::RegistrationListener::onNewDataMessage
(
	eprosima::fastrtps::Subscriber *sub
)
{
	eprosima::fastrtps::SampleInfo_t info;
	CommandRegisterMsg msg;
	if(sub->takeNextData(&msg, &info))
	{
		std::shared_ptr<RemoteCommand> pCommand =
			std::make_shared<RemoteCommand>
			(
				msg
			);

		{
			std::lock_guard<std::mutex> lock(owner.remote_commands_mutex);
			owner.remote_commands.push_back(pCommand);
		}

		if(owner.onNewCommand)
		{
			owner.onNewCommand(pCommand);
		}
	}
}
