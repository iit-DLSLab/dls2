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
		uint nArg_,
		uint level_,
		bool enabled_
	):
		name(name_),
		owner(owner_),
		doc(doc_),
		numArg(nArg_),
		level(level_),
		enabled(enabled_)
	{}

	std::string CommandBase::getName(){
		return this->name;
	}

	void CommandBase::setName(std::string name_){
		this->name = name_;
	}

	std::string CommandBase::getOwner(){
		return this->owner;
	}

	void CommandBase::setOwner(std::string owner_){
		this->owner = owner_;
	}

	std::string CommandBase::getDoc(){
		return this->doc;
	}

	void CommandBase::setDoc(std::string doc_){
		this->doc = doc_;
	}

	uint CommandBase::getLevel(){
		return this->level;
	}

	void CommandBase::setLevel(uint level_){
		this->level = level_;
	}

	uint CommandBase::getNumArgs(){
		return this->numArg;
	}

	void CommandBase::setEnabled(){
		this->enabled = true;
	}

	void CommandBase::setDisabled(){
		this->enabled = false;
	}

	bool CommandBase::isEnabled(){
		return this->enabled;
	}

	int CommandBase::call(std::vector<std::string>)	{
		return 0;
	}

} // end namespace dls

