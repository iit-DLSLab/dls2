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
        , level(0)
		, levelThread(&CommandManager::levelWatcher, this)
        , should_exit(false)
	{
		commands_monitor = new dls::DDSWriter(
			owner_+"::commands_monitor",
			domains::command,
			dls::topics::command_call
		);
	}

	CommandManager::~CommandManager()
	{
		{
			std::unique_lock<std::mutex> lock(this->levelMutex);
			this->should_exit.store(true);
		}

		for(auto elem : this->commands){
			elem.second->deactivate();
		}
		delete commands_monitor;

		this->levelCondVar.notify_one();
		this->levelThread.join();
	}

	// -----------------------------------------------------------------------------
	// Implementation
	// -----------------------------------------------------------------------------

	std::multimap<std::string, std::string> CommandManager::findByOwner
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

		return cmds;
	}

	std::multimap<std::string, std::string> CommandManager::findByName
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

		return cmdlst;
	}

	std::multimap<std::string, std::string> CommandManager::find
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

		return cmds;
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

	int CommandManager::callCommand(std::string name_, std::vector<std::string> args_, std::string owner_){

		std::multimap<std::string, std::string> cmdList;

		// find the commands
		if(owner_ == ""){
			cmdList = this->findByName(name_);
		}
		else{
			cmdList = this->find(owner_, name_);
		}

		if (cmdList.size() != 1)
			return cmdList.size();

		if (cmdList.begin()->first == this->getOwner()){
			auto cmd = this->commands.find(name_);
			cmd->second->call(args_);
			this->changeLevel(cmd->second->getNextLevel(this->getCurrentLevel()));
		}
		else{
			this->sendMessage(*(cmdList.begin()), args_);
		}
		
		return 1;
	}

	void CommandManager::sendMessage(std::pair<std::string, std::string> cmdData_, std::vector<std::string> args_)
	{
		CommandCallMsg msg;
		std::string outString;

		if (args_.empty()){
			outString.append(" ");
		}
		else{
			for (auto elem : args_){
				outString.append(elem);
				outString.append(",");
			}
		}

		msg.owner(cmdData_.second);
		msg.command_name(cmdData_.first);
		msg.args(outString);

		this->commands_monitor->sendMessage(&msg);
	}

	std::string CommandManager::getOwner(){
		return this->owner;
	}

	uint CommandManager::getCurrentLevel(){
		return this->level;
	}

	void CommandManager::changeLevel(uint level_){
		{
            std::unique_lock<std::mutex> lock(this->levelMutex);
            this->level = level_; 
        }
        this->levelCondVar.notify_one();
	}

	void CommandManager::verifyLevel(){

		for(auto cmd : this->commands){
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			if(cmd.second->testLevel(this->level)){
				cmd.second->activate();
			}
			else{
				cmd.second->deactivate();
			}
		}
	}

	void CommandManager::levelWatcher() 
    {
        while(true)
        {
            std::unique_lock<std::mutex> lock(this->levelMutex);
			this->levelCondVar.wait(lock);
			if(this->should_exit.load())
				break;
            this->verifyLevel();
        }
    }

    void CommandManager::disableCommand(std::string name)
    {
        if(commands.contains(name))
            this->commands[name]->deactivate();
    }

    void CommandManager::enableCommand(std::string name)
    {
        if(commands.contains(name))
            this->commands[name]->activate();
    }

} // end namespace dls
#endif /* end of include guard: COMMAND_MANAGER_CPP */
