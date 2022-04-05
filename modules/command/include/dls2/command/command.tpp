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
#include "dls2/topics/command_call.hpp"
#include "dls2/topics/command_register.hpp"
#include <fastrtps/types/DynamicTypeBuilderFactory.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/types/DynamicData.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <tuple>

#include "dls2/util/messaging/participant.hpp"

namespace dls
{
// =============================================================================
// Constructors
// =============================================================================
template <typename ret_t, typename...arg_ts>
Command<ret_t, arg_ts...>::Command
(
	const std::string &owner_,
	const std::string &command_name_,
	const std::string &docstring_,
	const std::function<ret_t(arg_ts...)>&f_,
	const bool remote
) :
	CommandBase(
		owner_,
		command_name_,
		docstring_,
		sizeof...(arg_ts)
	),
	f(f_)
	//msg(buildMsg(owner_, command_name_, docstring_)),
{
	if (remote){
		this->makeRemote();
	}
}

template <typename ret_t, typename...arg_ts>
Command<ret_t, arg_ts...>::~Command()
{
	//requestDeregistration();
}

template<class T> T transform_arg(std::string const &s){return s;}
// template<> double transform_arg(std::string const &s) { return atof(s.c_str());}
// template<> int transform_arg(std::string const &s) { return atoi(s.c_str());}
//template<> std::string transform_arg(std::string const &s) { return s;}


template <typename... Args, std::size_t... Is>
auto create_tuple_impl(std::index_sequence<Is...>, const std::vector<std::string>& arguments) {
    return std::make_tuple(transform_arg<Args>(arguments[Is])...);
}

template <typename... Args>
auto create_tuple(const std::vector<std::string>& args) {
    return create_tuple_impl<Args...>(std::index_sequence_for<Args...>{}, args);
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

template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::makeRemote()
{
	ddslink = std::make_shared<dls::DDSParticipant>(
		this->getCommandName(),
		dls::domains::command
	);

	// dls::topics::command_call,
	// version2::Subscriber<CommandRegisterMsgPubSubType>::CallbackType
	// (
	// 	[&](CommandRegisterMsg tuple)
	// 	{
	// 		//this->call(tuple);
	// 	}
	// )
}

template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::makeLocal()
{}

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

template <typename ret_t, typename...arg_ts>
unsigned int Command<ret_t, arg_ts...>::getNumArgs(){
	return sizeof...(arg_ts);
}
/*
// ========================== Argument Tuple Building ==========================
template <typename ret_t, typename...arg_ts>
template <typename tuple_arg1_t, typename tuple_arg2_t, typename... tuple_arg_ts>
std::tuple<tuple_arg1_t, tuple_arg2_t, tuple_arg_ts...>
	Command<ret_t, arg_ts...>::CommandCallListener::buildArgTuple
(
	size_t index
)
{
	std::tuple<tuple_arg1_t>
		t
		(
			takeArg
			<
				// typename std::remove_const
				// <
					typename std::remove_reference<tuple_arg1_t>::type
				// >::type
			>
			(pData, index)
		);
	return std::tuple_cat(t, buildArgTuple<tuple_arg2_t, tuple_arg_ts...>(++index));
}

template <typename ret_t, typename...arg_ts>
template <typename tuple_arg_t>
std::tuple<tuple_arg_t> Command<ret_t,
	arg_ts...>::CommandCallListener::buildArgTuple
(
	size_t index
)
{
	std::tuple<tuple_arg_t>
		t
		(
			takeArg
			<
				// typename std::remove_const
				// <
					typename std::remove_reference<tuple_arg_t>::type
				// >::type
			>
			(pData, index)
		);

	return t;
}
*/

} // end namespace dls
#endif /* end of include guard: COMMAND_TPP_OVALZHX0 */
