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
	template <typename ret_t, typename... arg_ts>
	void CommandManager::addCommand
	(
		std::string name_,
		std::string doc_,
		const std::function<ret_t(arg_ts...)> &f_,
		uint level_,
		bool enabled_
	)
	{
		this->commands.emplace_back
		(
			std::make_unique<Command<ret_t, arg_ts...>>
			(
				name_,
				this->owner,
				doc_,
				f_,
				level_,
				enabled_
			)
		);
	}

} // end namespace dls
#endif /* end of include guard: COMMAND_TPP_MANAGER */
