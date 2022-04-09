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
	owner(owner_)
	// ,
	// registration_listener(
	// 	dls::domains::layer,
	// 	"command_manager",
	// 	"command_registration",
	// 	nullptr //put here the callback for new commands from remote
	// )
{ }

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
		std::lock_guard<std::mutex> lock(this->commands_mutex);
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
	const std::string &name
) const
{
	std::vector<std::shared_ptr<CommandBase>> vec;
	{
		std::lock_guard<std::mutex> lock(this->commands_mutex);
		for(const auto &el : this->commands)
		{
			if(el->getCommandName() == name)
			{
				vec.push_back(el);
			}
		}
	}
	return vec;
}

std::shared_ptr<CommandBase> CommandManager::find
(
	const std::string &owner,
	const std::string &name
) const
{
	{
		std::lock_guard<std::mutex> lock(this->commands_mutex);
		for(const auto &el : this->commands)
		{
			if
			(
				el->getCommandOwner() == owner &&
				el->getCommandName() == name
			)
			{
				return el;
			}
		}
	}
	return nullptr;
}

std::vector<std::string> CommandManager::getCurrentlyRegisteredCommands(){
	//std::lock_guard<std::mutex> lock(this->commands_mutex);
	
	std::vector<std::string> remCommands;

	for(auto it = commands.begin(); it != commands.end(); ++it) {
		remCommands.push_back((*it)->getCommandName());
	}
		
	auto participant = eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->lookup_participant(domains::command);
	if (participant != NULL){
		//std::cout << participant->get_qos().name() << std::endl;
		remCommands = participant->get_participant_names();
		std::cout << remCommands.size() << std::endl;
	}

	return remCommands;
}

std::set<std::string> CommandManager::getCurrentlyRegisteredOwners()
{
	std::lock_guard<std::mutex> lock(this->commands_mutex);
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
	const std::string &owner,
	const std::string &name
) const
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
			std::unique_lock<std::mutex> lock(this->commands_mutex);
			this->command_added.wait(lock);
		}
	}while(pCommand == nullptr);

	RemoteCommandCallable callable;
	callable.pRemote_command = pCommand;
	return callable;
}



} // end namespace dls
