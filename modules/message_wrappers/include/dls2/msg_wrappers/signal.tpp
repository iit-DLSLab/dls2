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
#ifndef SIGNAL_TPP
#define SIGNAL_TPP

#include "dls2/msg_wrappers/signal.hpp"

using namespace dls;

template <typename SignalType>
Signal<SignalType>::Signal(const std::string& ID_, dls::DDSParticipant* participant, const std::shared_ptr<robotlib::RobotBase>& pRobot)
	: ID(ID_)
	, ddsLink(participant)
	, signal(pRobot)
{ }
	
template <typename SignalType>
Signal<SignalType>::~Signal()
{ }	

template <typename SignalType>
SignalType* Signal<SignalType>::operator->() 
{
	std::lock_guard<std::mutex> lock(this->signal_mutex);
    return &this->signal;
}

template <typename SignalType>
std::string Signal<SignalType>::getID()
{
	return this->ID_;
}

template <typename SignalType>
SignalType Signal<SignalType>::getData()
{
	return this->signal;
}

#endif /* end of include guard: SIGNAL_TPP */