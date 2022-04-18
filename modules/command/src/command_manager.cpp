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
#include "dls2/command/command_manager.hpp"
#include "dls2/msg/console_commandPubSubTypes.h"
#include <iostream>
#include <numeric>
#include <string_view>
#include <vector>

namespace dls
{


// =============================================================================
// Command Manager Implementation
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
CommandManager::CommandManager(std::string owner_):
	commands(),
	owner(owner_),
	commands_monitor(std::make_unique<dls::DDSParticipant>(owner_+"::commands_monitor", domains::command))
{
	commands_monitor->addWriter(dls::topics::command_call);
}

CommandManager::~CommandManager()
{ }

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
std::vector<std::shared_ptr<CommandBase>> CommandManager::findByOwner
(
	const std::string &owner
) const
{
	std::vector<std::shared_ptr<CommandBase>> vec;
	{
		for(const auto &el : this->commands)
		{
			if(el->getCommandOwner() == owner)
			{
				vec.push_back(el);
			}
		}
	}
	return vec;
}

std::vector<std::shared_ptr<CommandBase>> CommandManager::findByName
(
	std::string name_
)
{
	auto cmds = getCurrentlyRegisteredCommands();
	auto cmd = *std::find_if(cmds.begin(), cmds.end(), [&](auto el) { return (el.first == name_); });

	std::vector<std::shared_ptr<CommandBase>> vec;
	
	if (cmd.second == this->owner){
		for(const auto &el : this->commands){
			if(el->getCommandName() == name_)
			{
				vec.push_back(el);
			}
		}
	}
	else{
		vec.push_back(std::make_shared<Command<void>> (
			cmd.second,
			cmd.first,
			"doc string",
			std::function<void()>
			{
				[&]()
				{
					std::cout << "send data to execution" << std::endl;
				}
			}, 
			false
		));
	}

	return vec;
}

std::shared_ptr<CommandBase> CommandManager::find
(
	std::string owner_,
	std::string name_
)
{
	auto cmds = getCurrentlyRegisteredCommands();

	cmds.find(std::make_pair(name_, owner_));

	// verify if the command is local or remote
	if (owner_ == this->owner){
		return *std::find_if(commands.begin(), commands.end(), [&](auto el) { return (el->getCommandOwner() == owner_ &&
			el->getCommandName() == name_); });
	}
	else{
		std::cout << "TBD - make cmd object for remote command" << std::endl;
	}

	return nullptr;
}

std::set<std::pair<std::string, std::string>> CommandManager::getCurrentlyRegisteredCommands(){

	// get all the remote commands
	auto remCommands = commands_monitor->getParticipants();
	std::erase_if(remCommands, [](std::string value) { return (value.find("monitor") != std::string::npos); });

	// sort the remote commands
	std::set<std::pair<std::string, std::string>> cmds;

	for(auto elem :remCommands){
		size_t idx = elem.find("::");
		if (idx != std::string::npos)
			cmds.insert(std::make_pair(elem.substr(idx+2, elem.size()), elem.substr(0, idx)));
	}
		
	for(auto it : this->commands) {
		cmds.insert(std::make_pair(it->getCommandName(), it->getCommandOwner()));
	}

	return cmds;
}

std::set<std::string> CommandManager::getCurrentlyRegisteredOwners()
{
	auto remCommands = commands_monitor->getParticipants();


	std::set<std::string> set;
	for(auto it = commands.begin(); it != commands.end(); ++it)
	{
		set.insert((*it)->getCommandOwner());
	}

	return set;
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

RemoteCommandCallable CommandManager::makeCallable
(
	std::string owner,
	std::string name
)
{
	std::shared_ptr<CommandBase> pCommand;
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
		}
	}while(pCommand == nullptr);

	RemoteCommandCallable callable;
	callable.pRemote_command = pCommand;
	return callable;
}



} // end namespace dls
