#ifndef COMMAND_MANAGER_TPP
#define COMMAND_MANAGER_TPP

#include "dls2/command/command_manager.hpp"
#include "dls2/command/command.hpp"
#include "dls2/command/command_new.hpp"

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

	template<class T>
	void CommandManager::addCommand
	(
		std::string name_,
		std::string doc_,
		bool(T::*function_ptr)(),
		T* obj,
		dls::CommandBase::LevelType level_,
		bool enabled_
	)
	{
		std::shared_ptr<CommandBase> cmd = std::make_shared<CommandNew>
		(
			name_,
			this,
			doc_,
			std::bind(function_ptr, obj),
			level_,
			enabled_
		);

		if(cmd->testLevel(this->level))
			cmd->activate();

		this->commands.insert({name_, cmd});
	}

} // end namespace dls

#endif /* end of include guard: COMMAND_MANAGER_TPP */