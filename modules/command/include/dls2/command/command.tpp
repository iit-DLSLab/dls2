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
#ifndef COMMAND_TPP_OVALZHX0
#define COMMAND_TPP_OVALZHX0

// =============================================================================
// Includes
// =============================================================================
#include "dls2/command/command.hpp"
#include "dls2/topics/topics.hpp"
#include "dls2/util/messaging/dds_reader.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <tuple>
#include <type_traits>

namespace dls
{
	// =============================================================================
	// Constructors
	// =============================================================================
	template <typename ret_t, typename...arg_ts>
	Command<ret_t, arg_ts...>::Command
	(
		std::string name_,
		CommandManager *owner_,
		std::string docstring_,
		std::function<ret_t(arg_ts...)> f_,
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
		f(f_)
	{
	}

	template <typename ret_t, typename...arg_ts>
	Command<ret_t, arg_ts...>::~Command(){}


	template <typename ret_t, typename...arg_ts>
	void Command<ret_t, arg_ts...>::setEnabled(){
		if(this->isEnabled())
			return;

		this->registerCommand();
		this->enabled = true;
	}

	template <typename ret_t, typename...arg_ts>
	void Command<ret_t, arg_ts...>::setDisabled(){
		if(this->isEnabled())
			return;

		this->unregisterCommand();
		this->enabled = false;
	}

	template <typename ret_t, typename...arg_ts>
	void Command<ret_t, arg_ts...>::activate(){
		if(!this->isEnabled() || this->isActive())
			return;

		this->registerCommand();
		this->active = true;
	}

	template <typename ret_t, typename...arg_ts>
	void Command<ret_t, arg_ts...>::deactivate(){

		if(!this->isEnabled() || !this->isActive() || (this->ddslink == nullptr)) 
			return;

		delete this->ddslink;
		this->ddslink = nullptr;
		this->active = false;
	}

	template <typename ret_t, typename... arg_ts>
	int Command<ret_t, arg_ts...>::call(std::vector<std::string> args){
		// ensure args are correct size

		if (args.size() != this->getNumArgs()){
			std::cout << "Error: incorrect number of arguments" << std::endl;
			return 0;
		}

		auto arguments = create_tuple<arg_ts...>(args);
		//static_assert(std::is_same_v<decltype(arguments), const std::tuple<arg_ts...>>);
			
		std::apply(this->f, arguments);
		this->owner->changeLevel(this->getNextLevel(this->owner->getCurrentLevel()));

		return 1;
	}

	template <typename ret_t, typename... arg_ts>
	void Command<ret_t, arg_ts...>::registerCommand(){

		if (ddslink != nullptr)
			return;
				
		ddslink = new dls::DDSReader(
			this->getOwner() + "::" + this->getName(),
			dls::domains::command,
			topics::command_call,
			std::function<void(void *)>
			{
				[&](void *tuple)
				{
					CommandCallMsg msg = *((CommandCallMsg *)tuple);

					if (msg.owner() != this->getOwner() || msg.command_name() != this->getName())
						return;

					std::vector<std::string> result;

					//parse args
					size_t pos = 0;
					while ((pos = msg.args().find(",")) != std::string::npos) {
						result.push_back( msg.args().substr(0, pos) );
						msg.args().erase(0, pos + 1);
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
	auto create_tuple_impl(std::index_sequence<Is...>, const std::vector<std::string>& arguments) {
		return std::make_tuple(transform_args<Args>(arguments[Is])...);
	}

	template <typename... Args>
	auto create_tuple(const std::vector<std::string>& args) {
		return create_tuple_impl<Args...>(std::index_sequence_for<Args...>{}, args);
	}
} // end namespace dls
#endif /* end of include guard: COMMAND_TPP_OVALZHX0 */
