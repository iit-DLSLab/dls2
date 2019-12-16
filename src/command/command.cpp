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
#include "command/command.hpp"
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

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------

// =============================================================================
// Remote Command Manager Constructors
// =============================================================================
RemoteCommandManager::RemoteCommandManager
(
	std::function<void(std::shared_ptr<RemoteCommand>)> onNewCommand_,
	std::function<void(std::shared_ptr<RemoteCommand>)> onRemoveCommand_
) :
	registration_listener(*this),
	onNewCommand(onNewCommand_),
	onRemoveCommand(onRemoveCommand_)
{ }

// =============================================================================
// Remote Command Manager Implementation
// =============================================================================
// -----------------------------------------------------------------------------
// Finders
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

// =============================================================================
// Remote Command Mnager Helper Subscriber
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
RemoteCommandManager::RegistrationListener::RegistrationListener
(
	RemoteCommandManager &owner_
) :
	SubscriberBase<CommandRegisterMsgPubSubType>(topics::command_register),
	owner(owner_)
{ }

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
void RemoteCommandManager::RegistrationListener::onNewDataMessage
(
	eprosima::fastrtps::Subscriber *sub
)
{
	eprosima::fastrtps::SampleInfo_t info;
	CommandRegisterMsg msg;
	if(sub->takeNextData(&msg, &info))
	{
		std::cout << "Command manger got a new command" << std::endl;
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

