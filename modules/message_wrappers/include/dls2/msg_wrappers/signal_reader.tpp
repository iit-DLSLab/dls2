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
#ifndef SIGNAL_READER_TPP
#define SIGNAL_READER_TPP

#include "dls2/msg_wrappers/signal_reader.hpp"

#include <experimental/random>

using namespace dls;

template <typename SignalType>
SignalReader<SignalType>::SignalReader(std::shared_ptr<dls::DDSParticipant> participant_, const dls::topicType& topic_, const std::shared_ptr<SignalType> signal_)
	: Signal<SignalType>(participant_, signal_)
	, received(false)
{
	int id = std::experimental::randint(100000, 999999);
	while(participant_->getReader(std::to_string(id)) != nullptr)
		id = std::experimental::randint(100000, 999999);

	this->ID_ = std::to_string(id);

	this->ddsLink->addReader(this->ID_,
		topic_,
		std::function<void(void*)>
		{
			[&](void* tuple)
			{
				std::lock_guard<std::mutex> lock(this->signal_mutex);
				this->signal->loadMsg(tuple);
				received = true;
			}
		}
	);
}
	
template <typename SignalType>
SignalReader<SignalType>::~SignalReader()
{ 
	this->ddsLink->deleteReader(this->ID_);
}	

#endif /* end of include guard: SIGNAL_READER_TPP */