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
#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

// =============================================================================
// Includes
// =============================================================================
#include "dls2/util/messaging/participant.hpp"
#include "dls2/command/command.hpp"
#include "dls2/topics/topics.hpp"

#include <string>
#include <utility>
#include <vector>

namespace dls
{

// =============================================================================
// Command Manager Class
// =============================================================================
/// A class that manages commands
///
/// This class updates its internal representations as commands advertise
/// themselves and remove themselves from the framework
class CommandManager
{
public:
	/// Constructor
	///
	CommandManager(std::string owner_);

	/// Destructor
	///
	~CommandManager();

	/// Find commands by the owner
	///
	std::vector<std::shared_ptr<CommandBase>> findByOwner( std::string owner);

	/// Find commands by the name
	///
	std::vector<std::shared_ptr<CommandBase>> findByName( std::string name);

	/// Find command by the pair {owner, name}
	///
	std::vector<std::shared_ptr<CommandBase>> find( std::string owner, std::string name);

	/// Get list of all enabled commands in distrubuted framework
	///
	std::multimap<std::string, std::string> getCommandsList();

	/// Get a list of the unique owners of the commands
	///
	std::set<std::string> getCurrentlyRegisteredOwners();

	/// Adds a command to the CommandManager
	///
	/// @param name command name as seen by the rest of the framework
	/// @param doc some documentation for the command
	/// @param f the function encapsulated by the command
	/// @param level execution level of the command
	/// @param enabled set command enabled state
	template <typename ret_t, typename... arg_ts>
	void addCommand
	(
		std::string name,
		std::string doc,
		const std::function<ret_t(arg_ts...)> &f,
		uint level = 0,
		bool enabled = false
	);

	/// Removes a command from the manager
	///
	void removeCommand(CommandBase);

	/// Sends message over the framework distributed system
	///
	void sendMessage(void *msg);


private:

	/// Maque a list of callable commands
	///
	std::vector<std::shared_ptr<CommandBase>> makeCallableCmdList(
		std::multimap<std::string, std::string> cmdlst
	);

	/// Storage space for the commands
	///
	std::vector<std::shared_ptr<CommandBase>> commands;

	/// Owner layer of the commands
	///
	std::string owner;

	/// fastdds remote commands monitor
	///
	std::unique_ptr<dls::DDSParticipant> commands_monitor;	

};

} // end namespace dls

#include "dls2/command/command_manager.tpp"

#endif /* end of include guard: COMMAND_MANAGER_HPP */
