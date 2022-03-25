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

#include "dls2/util/messaging/participant.hpp"

// The current release of Eigen defines in the file `Eigen_Colamd.h` the macro:
// # define ALIVE (0)
// This macro seems to be refered to in only that file. Being a very generic
// name, it clashes with the enumeration in: `eprosima::fastrtps::rtps::ALIVE`
// As of this writing, it seems that this has been fixed in Eigen's github.
// However, it has not been officially released yet. At the time of this writing
// (21 January 2020), the latest stable release of Eigen is Eigen 3.3.7,
// released on 11 December 2018. Until such time as Eigen's fix makes it into a
// new release, the following lines solves the issue.
//
// For this message, and other similar issues, refer to the wiki page `Software
// Issues`

// #ifdef ALIVE
// #undef ALIVE
// #endif

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
	const std::function<ret_t(arg_ts...)>&f_
) :
	owner(owner_),
	command_name(command_name_),
	docstring(docstring_),
	f(f_),
	//msg(buildMsg(owner_, command_name_, docstring_)),
	commandSub(
			dls::domains::command,
			command_name_,
			dls::topics::command_call,
			version2::Subscriber<CommandRegisterMsgPubSubType>::CallbackType
			(
				[&](CommandRegisterMsg tuple)
				{
					this->call(tuple);
				}
			)
	)
{
	//requestRegistration();
}

template <typename ret_t, typename...arg_ts>
Command<ret_t, arg_ts...>::~Command()
{
	//requestDeregistration();
}

// -----------------------------------------------------------------------------
// Constructor Helpers
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

// =============================================================================
// Implementation
// =============================================================================
// -----------------------------------------------------------------------------
// Registration
// -----------------------------------------------------------------------------
/*template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::requestRegistration()
{
	auto msg = this->msg;
	msg.register_nremove() = true;
	//this->publisher.publish(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::requestDeregistration()
{
	auto msg = this->msg;
	msg.register_nremove() = false;
	//this->publisher.publish(msg);
}*/

// -----------------------------------------------------------------------------
// Calling
// -----------------------------------------------------------------------------
template <typename ret_t, typename...arg_ts>
ret_t Command<ret_t, arg_ts...>::call(std::tuple<arg_ts...> &t)
{
	static constexpr auto tuple_size = std::tuple_size<std::tuple<arg_ts...>>::value;
	return call(t, std::make_index_sequence<tuple_size>{});
}

template <typename ret_t, typename...arg_ts>
template<size_t...I>
ret_t Command<ret_t, arg_ts...>::call(std::tuple<arg_ts...> &t, std::index_sequence<I...>)
{
	return this->f(std::get<I>(t)...);
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
