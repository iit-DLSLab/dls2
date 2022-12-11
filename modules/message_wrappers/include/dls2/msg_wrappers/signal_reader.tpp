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

using namespace dls;

template <typename SignalType>
SignalReader<SignalType>::SignalReader(const std::string& ID_, dls::DDSParticipant* participant, const dls::topicType& topic_, const std::shared_ptr<robotlib::RobotBase>& pRobot)
	: Signal<SignalType>(ID_, participant, pRobot)
{
	this->ddsLink->addReader(ID_,
		topic_,
		std::function<void(void*)>
		{
			[&](void* tuple)
			{
				std::lock_guard<std::mutex> lock(this->signal_mutex);		
				this->signal.loadMsg(tuple);
			}
		}
	);
}
	
template <typename SignalType>
SignalReader<SignalType>::~SignalReader()
{ }	

#endif /* end of include guard: SIGNAL_READER_TPP */