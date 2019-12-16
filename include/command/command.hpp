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
#ifndef COMMAND_HPP_RSTXNA3I
#define COMMAND_HPP_RSTXNA3I

// =============================================================================
// Includes
// =============================================================================
#include "util/messaging/publisher_base.hpp"
#include "msg/command_registerPubSubTypes.h"
#include <string>
#include <functional>

namespace dls
{
// =============================================================================
// Class Interface
// =============================================================================
template <typename ret_t, typename...arg_ts>
class Command
{
public:
	Command
	(
		const std::string &command_name,
		const std::string &docstring,
		const std::function<ret_t(arg_ts...)> &f
	);

private:
	const std::string command_name;
	const std::string docstring;
	std::function<ret_t(arg_ts...)> f;
	PublisherBase<CommandRegisterMsgPubSubType> publisher;
};
} // end namespace dls

#include "command/command.tpp"

#endif /* end of include guard: COMMAND_HPP_RSTXNA3I */
