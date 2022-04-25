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
#ifndef COMMAND_BASE_CPP
#define COMMAND_BASE_CPP

#include "dls2/command/command_base.hpp"
#include "dls2/command/command_manager.hpp"

namespace dls
{
	// =============================================================================
	// Class Implementation
	// =============================================================================

	CommandBase::CommandBase(
		std::string name_,
		CommandManager *owner_,
		std::string doc_,
		uint nArg_,
		LevelType transitions_,
		bool enabled_
	)
		: name(name_)
		, owner(owner_)
		, doc(doc_)
		, numArg(nArg_)
		, transitionSet(transitions_)
		, enabled(enabled_)
		, active(false)
	{}

	std::string CommandBase::getName(){
		return this->name;
	}

	void CommandBase::setName(std::string name_){
		this->name = name_;
	}

	std::string CommandBase::getOwner(){
		return this->owner->getOwner();
	}

	std::string CommandBase::getDoc(){
		return this->doc;
	}

	void CommandBase::setDoc(std::string doc_){
		this->doc = doc_;
	}

	uint CommandBase::getNumArgs(){
		return this->numArg;
	}

	bool CommandBase::isEnabled(){
		return this->enabled;
	}

	void CommandBase::activate(){}
	void CommandBase::deactivate(){}

	bool CommandBase::isActive(){
		return this->active;
	}

	int CommandBase::call(std::vector<std::string>)	{
		return 0;
	}

	int CommandBase::getNextLevel(uint curLevel)
	{
		return this->transitionSet.find(curLevel)->second;
	}

	std::set<uint> CommandBase::getActiveLevels()
	{
		std::set<uint> activeLevels; 
		for(auto elem : transitionSet){
			activeLevels.insert(elem.first);
		}

		return activeLevels;
	}

	bool CommandBase::testLevel(uint level_)
	{
		return (this->transitionSet.find(level_) != this->transitionSet.end());
	}

} // end namespace dls
#endif /* end of include guard: COMMAND_BASE_CPP */

