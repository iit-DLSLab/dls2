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
#ifndef SIGNAL_WRITER_HPP
#define SIGNAL_WRITER_HPP

#include "dls2/msg_wrappers/signal.hpp"

namespace dls
{
	template <typename SignalType>
	class SignalWriter : public Signal<SignalType>
	{
	public:
		SignalWriter(dls::DDSParticipant*, const dls::topicType&, const std::shared_ptr<SignalType>);
		~SignalWriter();
		
		void publish();

	private:
		std::string ID;
	};
} // end namespace dls

#include "dls2/msg_wrappers/signal_writer.tpp"

#endif /* end of include guard: SIGNAL_WRITER_HPP */
