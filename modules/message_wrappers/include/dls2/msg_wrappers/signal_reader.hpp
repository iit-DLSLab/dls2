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
#ifndef SIGNAL_READER_HPP
#define SIGNAL_READER_HPP

#include "dls2/msg_wrappers/signal.hpp"

namespace dls
{
	template <typename SignalType>
	class SignalReader : public Signal<SignalType>
	{
	public:
		SignalReader(std::shared_ptr<dls::DDSParticipant>, const dls::topicType&, const std::shared_ptr<SignalType>);
		~SignalReader();	
		bool received;
	};
} // end namespace dls

#include "dls2/msg_wrappers/signal_reader.tpp"

#endif /* end of include guard: SIGNAL_READER_HPP */