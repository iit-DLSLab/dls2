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
		std::string owner_,
		std::string docstring_,
		const std::function<ret_t(arg_ts...)> &f_,
		uint level_,
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
		if(enabled_)
			this->registerCommand();
	}

	template <typename ret_t, typename...arg_ts>
	Command<ret_t, arg_ts...>::~Command(){}


	template <typename ret_t, typename...arg_ts>
	void Command<ret_t, arg_ts...>::setEnabled(){
		if(this->isEnabled())
			return;

		this->registerCommand();
		this->setEnabled();
	}

	template <typename ret_t, typename...arg_ts>
	void Command<ret_t, arg_ts...>::setDisabled(){
		if(this->isEnabled())
			return;

		this->unregisterCommand();
		this->setDisabled();
	}

	template <typename ret_t, typename... arg_ts>
	int Command<ret_t, arg_ts...>::call(std::vector<std::string> args){

		// Ensure args are correct size
		if (args.size() != this->getNumArgs()){
			std::cout << "Error: incorrect number of arguments" << std::endl;
			return 0;
		}
		
		auto arguments = create_tuple<arg_ts...>(args);
		//static_assert(std::is_same_v<decltype(arguments), const std::tuple<arg_ts...>>);
			
		std::apply(this->f, arguments);

		return 1;
	}

	template <typename ret_t, typename... arg_ts>
	void Command<ret_t, arg_ts...>::registerCommand(){

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
					CommandCallMsg *msg = (CommandCallMsg *)tuple;
			
					if (msg->owner() != this->getOwner() ||
							msg->command_name() != this->getName())
						return;

					std::vector<std::string> result;				

					//find if there is any args 
					if (msg->args().find(',') != std::string::npos){
						std::stringstream ss(msg->args());

						while(ss.good()){
							std::string substr;
							getline( ss, substr, ',' );
							result.push_back( substr );
						}
					}

					this->call(result);
				}
			}
		);
	}

	template <typename ret_t, typename... arg_ts>
	void Command<ret_t, arg_ts...>::unregisterCommand()
	{
		if (ddslink == nullptr)
			return;
			
		delete ddslink;
	}


	// -----------------------------------------------------------------------------
	// Class Helpers
	// -----------------------------------------------------------------------------
	template <typename ret_t, typename...arg_ts>
	CommandRegisterMsg Command<ret_t, arg_ts...>::buildMsg
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring
	)
	{
		CommandRegisterMsg msg;
		msg.owner(owner);
		msg.command_name(command_name);
		msg.docstring(docstring);

		buildRepresentationVector<arg_ts...>(msg.arg_types());
		msg.ret_type
		(
			static_cast
			<
				std::remove_reference
				<
					decltype(msg.ret_type())
				>::type
			>
			(
				CommandBase::typeToRepresentation<ret_t>()
			)
		);
		return msg;
	}

	template<class T> T transform_args(std::string const &s){return s;}

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
