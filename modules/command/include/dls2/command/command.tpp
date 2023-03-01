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
#ifndef COMMAND_TPP
#define COMMAND_TPP

#include "dls2/command/command.hpp"
#include "dls2/topics/topics.hpp"
#include "dls2/util/messaging/dds_reader.hpp"

// #include <iostream>
// #include <thread>
// #include <chrono>
// #include <tuple>
// #include <type_traits>

namespace dls
{
	template <typename...arg_ts>
	Command<arg_ts...>::Command
	(
		std::string name_,
		CommandManager *owner_,
		std::string docstring_,
		std::function<bool(arg_ts...)> f_,
		dls::CommandBase::LevelType level_,
		bool enabled_
	) :
		CommandBase(
			name_,
			owner_,
			docstring_,
			sizeof...(arg_ts),
			level_,
			enabled_
		),
		f(f_),
		ddslink(nullptr)
	{ }

	template <typename... arg_ts>
	Command<arg_ts...>::~Command(){}

	template <typename... arg_ts>
	void Command<arg_ts...>::setEnabled()
	{
		if(this->isEnabled())
			return;

		this->registerCommand();
		this->enabled = true;
	}

	template <typename... arg_ts>
	void Command<arg_ts...>::setDisabled()
	{
		if(this->isEnabled())
			return;

		this->unregisterCommand();
		this->enabled = false;
	}

	template <typename... arg_ts>
	void Command<arg_ts...>::activate()
	{
		if(!this->isEnabled() || this->isActive())
			return;

		this->registerCommand();
		this->active = true;
	}

	template <typename... arg_ts>
	void Command<arg_ts...>::deactivate()
	{
		if(!this->isEnabled() || !this->isActive() || (this->ddslink == nullptr)) 
			return;

		this->ddslink.reset();
		this->active = false;
	}

	template <typename... arg_ts>
	bool Command<arg_ts...>::call(std::vector<std::string> args)
	{
		// ensure args are correct size

		if (args.size() != this->getNumArgs()){
			std::cerr << "Error: incorrect number of arguments" << std::endl;
			return false;
		}

		auto arguments = create_tuple<arg_ts...>(args);
		//static_assert(std::is_same_v<decltype(arguments), const std::tuple<arg_ts...>>);
			
		if(std::apply(this->f, arguments))
            CommandHelper::changeCommandLevel(this->owner, this->getNextLevel(CommandHelper::getCurrentLevel(this->owner)));

		return true;
	}

	template <typename... arg_ts>
	void Command<arg_ts...>::registerCommand()
	{
		if (ddslink != nullptr)
			return;
				
		ddslink = std::make_shared<dls::DDSReader>(
			this->getOwner() + "::" + this->getName(),
			dls::domains::command,
			topics::command_call,
			std::function<void(void *)>
			{
				[&](void *tuple)
				{
					CommandCallMsg* msg = (CommandCallMsg *)tuple;
					// if the owner is known but it is not the one owning this command OR
					// if the received command name is not equal to the name of this command
					// do nothing
					if ((msg->owner() != "" && msg->owner() != this->getOwner()) || msg->command_name() != this->getName())
						return;

					std::vector<std::string> result;

					//parse args
					size_t pos = 0;
					while ((pos = msg->args().find(",")) != std::string::npos) {
						result.push_back( msg->args().substr(0, pos) );
						msg->args().erase(0, pos + 1);
					}			

					this->call(result);
				}
			}
		);
	}


	// -----------------------------------------------------------------------------
	// Class Helpers
	// -----------------------------------------------------------------------------
	template<class T> T transform_args(std::string const &s);

	template <typename... Args, std::size_t... Is>
	auto create_tuple_impl(std::index_sequence<Is...>, const std::vector<std::string>& arguments)
	{
		return std::make_tuple(transform_args<Args>(arguments[Is])...);
	}

	template <typename... Args>
	auto create_tuple(const std::vector<std::string>& args)
	{
		return create_tuple_impl<Args...>(std::index_sequence_for<Args...>{}, args);
	}
} // end namespace dls

#endif /* end of include guard: COMMAND_TPP */
