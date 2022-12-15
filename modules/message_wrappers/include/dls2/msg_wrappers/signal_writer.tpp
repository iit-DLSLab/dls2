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
#ifndef SIGNAL_WRITER_TPP
#define SIGNAL_WRITER_TPP

#include "dls2/msg_wrappers/signal_writer.hpp"

using namespace dls;

template <typename SignalType>
SignalWriter<SignalType>::SignalWriter(dls::DDSParticipant* participant_, const dls::topicType& topic_, SignalType* signal_)
	: Signal<SignalType>(participant_, signal_)
{
	participant_->addWriter("signal_writer", topic_);
}
	
template <typename SignalType>
SignalWriter<SignalType>::~SignalWriter()
{ }	

template <typename SignalType>
void SignalWriter<SignalType>::publish()
{
	std::lock_guard<std::mutex> lock(this->signal_mutex);
	this->ddsLink->sendMessage("signal_writer", this->signal->getMsg());
}

#endif /* end of include guard: SIGNAL_WRITER_TPP */