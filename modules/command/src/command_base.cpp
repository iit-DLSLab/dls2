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
// =============================================================================
// Includes
// =============================================================================
#include "dls2/command/command_base.hpp"

namespace dls
{
	// =============================================================================
	// Class Implementation
	// =============================================================================

	CommandBase::CommandBase(
		std::string name_,
		std::string owner_,
		std::string doc_,
		uint nArg_
	):
		name(name_),
		owner(owner_),
		docstring(doc_),
		numArg(nArg_)
	{}

	std::string CommandBase::getCommandName(){
		return name;
	}

	std::string CommandBase::getCommandOwner(){
		return owner;
	}

	std::string CommandBase::getCommandDoc(){
		return docstring;
	}

	uint CommandBase::getNumArgs(){
		return numArg;
	}

	int CommandBase::call(std::vector<std::string>)	{
		return 0;
	}

} // end namespace dls

