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
SignalWriter<SignalType>::SignalWriter(const std::string& ID_, dls::DDSParticipant* participant, const dls::topicType& topic_, const std::shared_ptr<robotlib::RobotBase>& pRobot)
	: Signal<SignalType>(ID_, participant, pRobot)
{
	participant->addWriter(ID_, topic_);
}
	
template <typename SignalType>
SignalWriter<SignalType>::~SignalWriter()
{ }	

template <typename SignalType>
void SignalWriter<SignalType>::publish()
{
	std::lock_guard<std::mutex> lock(this->signal_mutex);
	this->ddsLink->sendMessage(this->ID, this->signal.getMsg());
}

#endif /* end of include guard: SIGNAL_WRITER_TPP */