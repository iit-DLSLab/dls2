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
	const std::string &command_name_,
	const std::string &docstring_,
	const std::function<ret_t(arg_ts...)>&f_
) :
	command_name(command_name_),
	docstring(docstring_),
	f(f_),
	publisher(topics::command_register)
{
	CommandRegisterMsg msg;

	msg.command_name(command_name_);
	msg.docstring(docstring_);
	// for(int i = 0; i != 10; ++i)
	// {
	// 	std::cout << "about to publish registration message" << std::endl;
		publisher.publish(msg);
	// }
}

} // end namespace dls
#endif /* end of include guard: COMMAND_TPP_OVALZHX0 */
