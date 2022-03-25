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
#ifndef COMMAND_MANAGER_TPP
#define COMMAND_MANAGER_TPP

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
// Implementation
// -----------------------------------------------------------------------------
template <typename ret_t, typename... arg_ts>
void CommandManager::addCommand
(
	const std::string &owner,
	const std::string &command_name,
	const std::string &docstring,
	const std::function<ret_t(arg_ts...)> &f
)
{
	this->commands.emplace_back
	(
		std::make_unique<Command<ret_t, arg_ts...>>
		(
			owner,
			command_name,
			docstring,
			f
		)
	);
}

} // end namespace dls
#endif /* end of include guard: COMMAND_TPP_MANAGER */
