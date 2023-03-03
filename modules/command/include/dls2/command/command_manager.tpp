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

#include "dls2/command/command_manager.hpp"
#include "dls2/command/command.hpp"

namespace dls
{
	template <typename... arg_ts>
	void CommandManager::addCommand
	(
		std::string name_,
		std::string doc_,
		const std::function<bool(arg_ts...)> &f_,
		dls::CommandBase::LevelType level_,
		bool enabled_
	)
	{
		std::shared_ptr<CommandBase> cmd = std::make_shared<Command<arg_ts...>>
		(
			name_,
			this,
			doc_,
			f_,
			level_,
			enabled_
		);

		if(cmd->testLevel(this->level))
			cmd->activate();

		this->commands.insert({name_, cmd});
		
	}

} // end namespace dls

#endif /* end of include guard: COMMAND_MANAGER_TPP */