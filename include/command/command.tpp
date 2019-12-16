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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef COMMAND_TPP_OVALZHX0
#define COMMAND_TPP_OVALZHX0

// =============================================================================
// Includes
// =============================================================================
#include "command/command.hpp"
#include "topics/command_register.hpp"
#include <iostream>
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
	msg(buildMsg(owner_, command_name_, docstring_)),
	publisher(topics::command_register)
{
	requestRegistration();
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
			typeToRepresentation<ret_t>()
		)
	);
	return msg;
}

template <typename arg1_t, typename arg2_t, typename...arg_other_ts>
CommandBase::RepresentationVector &CommandBase::buildRepresentationVector
(
	RepresentationVector &vec
)
{
	vec.push_back
	(
		static_cast
		<
			std::remove_reference
			<
				decltype(vec)
			>::type::value_type
		>
		(
			typeToRepresentation<arg1_t>()
		)
	);
	buildRepresentationVector<arg2_t, arg_other_ts...>(vec);
	return vec;
}

template <typename arg_t>
CommandBase::RepresentationVector &CommandBase::buildRepresentationVector
(
	RepresentationVector &vec
)
{
	vec.push_back
	(
		static_cast
		<
			std::remove_reference
			<
				decltype(vec)
			>::type::value_type
		>
		(
			typeToRepresentation<arg_t>()
		)
	);
	return vec;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<void>()
{
	return ArgumentType::VOID;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<char>()
{
	return ArgumentType::CHAR;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<uint8_t>()
{
	return ArgumentType::UINT8;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<int16_t>()
{
	return ArgumentType::INT16;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<uint16_t>()
{
	return ArgumentType::UINT16;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<int32_t>()
{
	return ArgumentType::UINT32;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<int64_t>()
{
	return ArgumentType::INT64;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<uint64_t>()
{
	return ArgumentType::UINT64;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<float>()
{
	return ArgumentType::FLOAT;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<double>()
{
	return ArgumentType::DOUBLE;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<long double>()
{
	return ArgumentType::LONG_DOUBLE;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<bool>()
{
	return ArgumentType::BOOL;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<std::string>()
{
	return ArgumentType::STD_STRING;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<const std::string&>()
{
	return ArgumentType::STD_STRING;
}

template <>
CommandBase::ArgumentType CommandBase::typeToRepresentation<std::string&>()
{
	return ArgumentType::STD_STRING;
}

// =============================================================================
// Implementation
// =============================================================================
template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::requestRegistration()
{
	this->publisher.publish
	(
		// const_cast<CommandRegisterMsg>
		// (
			this->msg
		// )
	);
}

template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::requestDeregistration()
{
	// TODO implement
}

// =============================================================================
// Remote Commands
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------

// =============================================================================
// Command Manager
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
CommandManager::CommandManager():
	commands()
{ }

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
#endif /* end of include guard: COMMAND_TPP_OVALZHX0 */
