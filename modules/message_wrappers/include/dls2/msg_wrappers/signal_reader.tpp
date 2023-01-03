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
SignalReader<SignalType>::SignalReader(dls::DDSParticipant* participant_, const dls::topicType& topic_, const std::shared_ptr<SignalType> signal_)
	: Signal<SignalType>(participant_, signal_)
{
	int id = std::experimental::randint(100000, 999999);
	while(participant_->getReader(std::to_string(id)) != nullptr)
		id = std::experimental::randint(100000, 999999);

	this->ID = std::to_string(id);

	this->ddsLink->addReader(this->ID,
		topic_,
		std::function<void(void*)>
		{
			[&](void* tuple)
			{
				std::lock_guard<std::mutex> lock(this->signal_mutex);
				this->signal->loadMsg(tuple);
			}
		}
	);
}
	
template <typename SignalType>
SignalReader<SignalType>::~SignalReader()
{ 
	this->ddsLink->deleteReader(this->ID);
}	

#endif /* end of include guard: SIGNAL_READER_TPP */