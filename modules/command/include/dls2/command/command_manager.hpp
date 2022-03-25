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
#include "dls2/command/command.hpp"
#include <string>
#include <utility>
#include <vector>


namespace dls
{
// =============================================================================
// Container Class
// =============================================================================
/// Command Manager class
///
/// External components should prefer using this class instead of directly using
/// `Command`
class CommandManager
{
public:
	/// Constructor
	///
	CommandManager();

	/// Destructor
	///
	~CommandManager();


	/// Adds a command to the CommandManager and registers it with the rest of
	/// the framework
	///
	/// @param owner name of the component that owns the command
	/// @command_name command name as seen by the rest of the framework
	/// @param docstring some documentation for the command
	/// @param f the function encapsulated by the command
	template <typename ret_t, typename... arg_ts>
	void addCommand
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring,
		const std::function<ret_t(arg_ts...)> &f
	);

private:
	/// Storage space for the commands
	///
	std::vector<std::unique_ptr<CommandBase>> commands;
};
} // end namespace dls

#include "dls2/command/command_manager.tpp"

#endif /* end of include guard: COMMAND_MANAGER_HPP */
