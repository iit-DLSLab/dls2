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

#include <experimental/random>

using namespace dls;

template <typename SignalType>
SignalWriter<SignalType>::SignalWriter(std::shared_ptr<dls::DDSParticipant> participant_, const dls::topicType& topic_, const std::shared_ptr<SignalType> signal_)
	: Signal<SignalType>(participant_, signal_)
{
	int id = std::experimental::randint(100000, 999999);
	while(participant_->getWriter(std::to_string(id)) != nullptr)
		id = std::experimental::randint(100000, 999999);

	this->ID = std::to_string(id);

	participant_->addWriter(this->ID, topic_);
}
	
template <typename SignalType>
SignalWriter<SignalType>::~SignalWriter()
{ 
	this->ddsLink->deleteWriter(this->ID);
}	

template <typename SignalType>
void SignalWriter<SignalType>::publish()
{
	std::lock_guard<std::mutex> lock(this->signal_mutex);
	this->ddsLink->sendMessage(this->ID, this->signal->getMsg());
}

#endif /* end of include guard: SIGNAL_WRITER_TPP */