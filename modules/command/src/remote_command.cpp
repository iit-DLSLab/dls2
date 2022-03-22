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
// =============================================================================
// Includes
// =============================================================================
#include <iostream>
#include <vector>

#include "dls2/topics/command_register.hpp"
#include "dls2/command/remote_command.hpp"
#include "dls2/topics/command_call.hpp"

#include <fastrtps/types/DynamicTypeBuilderFactory.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/types/DynamicTypeBuilderPtr.h>
#include <fastrtps/types/DynamicTypeBuilder.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/types/DynamicType.h>
#include <fastrtps/types/DynamicData.h>
#include <fastrtps/Domain.h>

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
RemoteCommand::RemoteCommand(const std::string &topic_, CommandRegisterMsg &msg) :
	owner(msg.owner()),
	command_name(msg.command_name()),
	docstring(msg.docstring()),
	args(msg.arg_types()),
	ret_type(msg.ret_type()),
	remote_command_publisher(topic_)
{ }

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------

void RemoteCommand::call() const
{
	CommandRegisterMsg msg;
	//TBD put here the message to be send
	this->remote_command_publisher.publish(msg);
}

// =============================================================================
// Remote Command Callable
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
RemoteCommandCallable::RemoteCommandCallable() :
	pRemote_command(nullptr)
{ }

// =============================================================================
// Remote Command Manager
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
RemoteCommandManager::RemoteCommandManager
(
	std::function<void(std::shared_ptr<const RemoteCommand>)> onNewCommand_,
	std::function<void(std::shared_ptr<const RemoteCommand>)> onRemoveCommand_
) :
	remote_commands_mutex(),
	remote_commands(),
	command_added(),
	registration_listener(
		"remote_command_manager",
		"command_registration",
		dls::domains::command_domain,
		nullptr //put here the callback for new commands from remote
	),  
	onNewCommand(onNewCommand_),
	onRemoveCommand(onRemoveCommand_)
{ }

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
std::vector<std::shared_ptr<const RemoteCommand>> RemoteCommandManager::findByOwner
(
	const std::string &owner
) const
{
	std::vector<std::shared_ptr<const RemoteCommand>> vec;
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

std::vector<std::shared_ptr<const RemoteCommand>> RemoteCommandManager::findByName
(
	const std::string &name
) const
{
	std::vector<std::shared_ptr<const RemoteCommand>> vec;
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

std::shared_ptr<const RemoteCommand> RemoteCommandManager::find
(
	const std::string &owner,
	const std::string &name
) const
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

std::vector<std::shared_ptr<const RemoteCommand>>
	RemoteCommandManager::getCurrentlyRegisteredCommands()
{
	std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
	return this->remote_commands;
}

std::set<std::string> RemoteCommandManager::getCurrentlyRegisteredOwners()
{
	std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
	std::set<std::string> set;
	for(auto it = remote_commands.begin(); it != remote_commands.end(); ++it)
	{
		set.insert((*it)->owner);
	}

	return set;
}

void RemoteCommandManager::addCommand(std::shared_ptr<RemoteCommand> pCommand)
{
	std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
	// check whether the command that is being registered has already been
	// registered. This can happen, for instance, if the user force quits a
	// layer. In that case, the layer will not be able to publish that it is
	// removing its commands. When the layer is restarted, it will try to
	// reregister all of its commands. Do not accept the duplicates.
	bool already_exists = false;


	// This loop is not the most effective way to do this, but it's the simplest
	// and this is not performance-critical code
	for(const auto &command : this->remote_commands)
	{
		if
		(
			command->owner        == pCommand->owner &&
			command->command_name == pCommand->command_name
		)
		{
			already_exists = true;
			break;
		}
	}
	if(!already_exists)
	{
		this->remote_commands.push_back(pCommand);
		this->command_added.notify_all();
	}

	if(this->onNewCommand)
	{
		this->onNewCommand(pCommand);
	}
}

void RemoteCommandManager::removeCommand(const CommandRegisterMsg &msg)
{
	std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
	for(auto it = this->remote_commands.begin(); it != this->remote_commands.end(); ++it)
	{
		if
		(
			(*it)->owner == msg.owner() &&
			(*it)->command_name == msg.command_name()
		)
		{
			if(this->onRemoveCommand)
			{
				this->onRemoveCommand(*it);
			}
			this->remote_commands.erase(it);
			break;
		}
	}
}

RemoteCommandCallable RemoteCommandManager::makeCallable
(
	const std::string &owner,
	const std::string &name
) const
{
	std::shared_ptr<const RemoteCommand> pCommand;
	do
	{
		pCommand = this->find
		(
			owner,
			name
		);
		if(!pCommand)
		{
			std::cout << "Command '" << name << "' for '" << owner <<
				"' not yet registered. Blocking" << std::endl;
			std::unique_lock<std::mutex> lock(this->remote_commands_mutex);
			this->command_added.wait(lock);
		}
	}while(pCommand == nullptr);

	RemoteCommandCallable callable;
	callable.pRemote_command = pCommand;
	return callable;
}

// -----------------------------------------------------------------------------
// Subscriber Helper
// -----------------------------------------------------------------------------
// =============================== Constructors ================================


// ============================== Implementation ===============================
void RemoteCommandManager::onNewDataMessage
(
	eprosima::fastrtps::Subscriber *sub
)
{
	eprosima::fastrtps::SampleInfo_t info;
	CommandRegisterMsg msg;
	if(sub->takeNextData(&msg, &info))
	{
		if(msg.register_nremove())
		{
			std::shared_ptr<RemoteCommand> pCommand =
				std::make_shared<RemoteCommand>
				(
					"new_command",
					msg
				);
			this->addCommand(pCommand);
		}
		else
		{
			this->removeCommand(msg);
		}
	}
}
