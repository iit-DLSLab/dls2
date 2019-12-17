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
			CommandBase::typeToRepresentation<ret_t>()
		)
	);
	return msg;
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
// Command Manager Implementation
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
