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
#ifndef COMMAND_MANAGER_CPP
#define COMMAND_MANAGER_CPP

// =============================================================================
// Includes
// =============================================================================
#include "dls2/command/command_manager.hpp"
#include <iostream>
#include <numeric>
#include <string_view>
#include <vector>
#include <sstream>

namespace dls
{
	// =============================================================================
	// Command Manager Implementation
	// =============================================================================
	// -----------------------------------------------------------------------------
	// Constructors
	// -----------------------------------------------------------------------------
	CommandManager::CommandManager(std::string owner_)
		: commands()
		, owner(owner_)
		, commands_monitor(std::make_unique<dls::DDSParticipant>(
			owner_+"::commands_monitor",
			domains::command)
		)
		, level(0)
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
		std::string owner_
	)
	{
		auto cmds = this->getCommandsList();

		if (cmds.size() == 0)
			return {};

		// search for commands with same owner
		std::multimap<std::string, std::string> cmdlst;
		for (auto it = cmds.begin(); it != cmds.end(); ++it)
		{
			if (it->second == owner_)
    			cmdlst.insert(*it);
		}

		return makeCallableCmdList(cmds);
	}

	std::vector<std::shared_ptr<CommandBase>> CommandManager::findByName
	(
		std::string name_
	)
	{
		auto cmds = this->getCommandsList();
		
		if (cmds.count(name_) == 0)
			return {};

		// find all commands with same name and put in a new multimap
		auto range = cmds.equal_range(name_);
		std::multimap<std::string, std::string> cmdlst;
		for (auto it = range.first; it != range.second; ++it)
		{
    		cmdlst.insert(*it);
		}

		return makeCallableCmdList(cmdlst);
	}

	std::vector<std::shared_ptr<CommandBase>> CommandManager::find
	(
		std::string owner_,
		std::string name_
	)
	{
		auto cmds = getCommandsList();

		if (cmds.count(name_) == 0)
			return {};

		// find all commands with same name and put in a new multimap
		// but now with keys being the owner
		auto range = cmds.equal_range(name_);
		std::multimap<std::string, std::string> cmdlst;
		for (auto it = range.first; it != range.second; ++it)
		{
    		cmdlst.insert({it->second, it->first});
		}

		if (cmdlst.count(owner_) == 0)
			return {};
			
		// find all commands with same owner and put in a new multimap
		// getting back the key invertion
		range = cmdlst.equal_range(owner_);
		cmds.clear();
		for (auto it = range.first; it != range.second; ++it)
		{
    		cmds.insert({it->second, it->first});
		}

		return makeCallableCmdList(cmds);
	}


	std::vector<std::shared_ptr<CommandBase>> CommandManager::makeCallableCmdList(
		std::multimap<std::string, std::string> cmdlst
	)
	{
		// create a list of commands that could be called
		std::vector<std::shared_ptr<CommandBase>> retvec;
		
		for(auto elem : cmdlst){
			//if the command is local
			if (elem.second == this->owner){
				for(const auto &el : this->commands){
					if(el.first == elem.first)
					{
						retvec.push_back(el.second);
					}
				}
			}
			//if the command is remote
			else{
				retvec.push_back(std::make_shared<Command<void, std::string>> (
					elem.first,
					elem.second,
					"temp remote command",
					std::function<void(std::string)>
					{
						[&](std::string args_)
						{
							// this call just send all the info to the remote command
							// the remote command should verify if the number of arguments and types ar correct
							CommandCallMsg msg;
							
							std::stringstream ss(args_);
							std::vector<std::string> result;

							while(ss.good()){
								std::string substr;
								getline( ss, substr, '#' );
								result.push_back( substr );
							}

							msg.owner(result[0]);
							msg.command_name(result[1]);
							msg.args(result[2]);

							this->commands_monitor->sendMessage(&msg);
						}
					}
				));
			}
		}

		return retvec;
	}

	std::multimap<std::string, std::string> CommandManager::getCommandsList(){

		// get all the remote commands
		auto remCommands = commands_monitor->getParticipants();
		// remove the monitors
		std::erase_if(remCommands, [](std::string value) { return (value.find("monitor") != std::string::npos); });

		// create a list of commands
		std::multimap<std::string, std::string> cmds;

		for(auto elem :remCommands){
			size_t idx = elem.find("::");
			if (idx != std::string::npos)
				cmds.insert({elem.substr(idx+2, elem.size()), elem.substr(0, idx)});
		}
			
		return cmds;
	}

	std::set<std::string> CommandManager::getOwnersList()
	{
		auto remCommands = commands_monitor->getParticipants();


		std::set<std::string> set;
		for(auto it = commands.begin(); it != commands.end(); ++it)
		{
			set.insert(it->second->getOwner());
		}

		return set;
	}

	void CommandManager::setCommandLevel(int level_){

		if(this->level == level_)
			return;

		for(auto cmd : this->commands){
			if(cmd.second->getLevel().find(level_) != cmd.second->getLevel().end()){
				cmd.second->activate();
			}
			else{
				cmd.second->desactivate();
			}
		}
	}

	int CommandManager::callCommand(std::string name_, std::vector<std::string> args_, std::string owner_){

		std::vector<std::shared_ptr<dls::CommandBase>> cmdList;

		// find the commands
		if(owner_ == ""){
			cmdList = this->findByName(name_);
		}
		else{
			cmdList = this->find(owner_, name_);
		}

		if (cmdList.size() != 1)
			return cmdList.size();
		
		// call the command
		cmdList.front()->call(this->prepareArgs(cmdList.front(), args_));

		// TBD implement next level and change current running level
		// cmdList.front()->getNextLevel();
		
		return 1;
	}


	std::vector<std::string> CommandManager::prepareArgs(std::shared_ptr<dls::CommandBase> cmd, std::vector<std::string> args )
	{
		//if the command is remote configure the message
		if (cmd->getOwner() == this->owner)
			return args;

		std::string outString;
		outString.append(cmd->getOwner());
		outString.append("#");
		outString.append(cmd->getName());
		outString.append("#");

		if (args.empty()){
			outString.append(" ");
		}
		else{
			for (auto elem : args){
				outString.append(elem);
				outString.append(",");
			}
		}

		return std::vector<std::string>({outString});
	}

} // end namespace dls

#endif /* end of include guard: COMMAND_MANAGER_CPP */
