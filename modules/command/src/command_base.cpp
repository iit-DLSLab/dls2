
#ifndef COMMAND_BASE_CPP
#define COMMAND_BASE_CPP

#include "dls2/command/command_base.hpp"
#include "dls2/command/command_manager.hpp"

using namespace dls;

CommandBase::CommandBase(
	std::string name_,
	CommandManager *owner_,
	std::string doc_,
	uint nArg_,
	LevelType transitions_,
	bool enabled_
)
	: transitionSet(transitions_)
	, enabled(enabled_)
	, active(false)
	, owner(owner_)
	, name(name_)
	, doc(doc_)
	, numArg(nArg_)
{ }

std::string CommandBase::getName()
{
	return this->name;
}

void CommandBase::setName(std::string name_)
{
	this->name = name_;
}

std::string CommandBase::getOwner()
{
	return this->owner->getOwner();
}

std::string CommandBase::getDoc()
{
	return this->doc;
}

void CommandBase::setDoc(std::string doc_)
{
	this->doc = doc_;
}

uint CommandBase::getNumArgs()
{
	return this->numArg;
}

bool CommandBase::isEnabled()
{
	return this->enabled;
}

bool CommandBase::isActive()
{
	return this->active;
}

bool CommandBase::call(std::vector<std::string>)
{
	return false;
}

bool CommandBase::call()
{
	return false;
}

int CommandBase::getNextLevel(uint curLevel)
{
	if(this->transitionSet.empty())
		return curLevel;

	return this->transitionSet.find(curLevel)->second;
}

bool CommandBase::testLevel(uint level_)
{
	if(this->transitionSet.empty())
		return true;
		
	return (this->transitionSet.find(level_) != this->transitionSet.end());
}

void CommandBase::changeTransitionSet(const LevelType& transition_set)
{
	this->transitionSet = transition_set;
}
#endif /* end of include guard: COMMAND_BASE_CPP */