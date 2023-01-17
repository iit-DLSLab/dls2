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
Signal<SignalType>::Signal(dls::DDSParticipant* participant_, const std::shared_ptr<SignalType> signal_)
	: ddsLink(participant_)
	, signal(signal_)
{ 
	static_assert(
    std::is_convertible<SignalType*, Wrapper<typename SignalType::type>*>::value,
    "Signal type should always derive (heritage) from dls::Wrapper class");
}
	
template <typename SignalType>
Signal<SignalType>::~Signal()
{ }	

template <typename SignalType>
std::shared_ptr<SignalType> Signal<SignalType>::operator->() 
{
	std::lock_guard<std::mutex> lock(this->signal_mutex);
    return this->signal;
}

template <typename SignalType>
SignalType Signal<SignalType>::getData()
{
	return *this->signal;
}

#endif /* end of include guard: SIGNAL_TPP */