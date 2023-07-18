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
#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <mutex>
#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/topics/topics.hpp"
#include "robotlib/robot_factory.hpp"
#include "dls2/msg_wrappers/wrapper.hpp"

namespace dls
{
	template <typename SignalType>
	class Signal
	{
	public:
		Signal(std::shared_ptr<dls::DDSParticipant>, const std::shared_ptr<SignalType>);
		Signal() = delete;
		~Signal();

		std::shared_ptr<SignalType> operator->();
		SignalType getData();
	
	protected:
		std::shared_ptr<dls::DDSParticipant> ddsLink;

		std::string ID_{};

		// BEGIN critical section
			const std::shared_ptr<SignalType> signal;
			mutable std::mutex signal_mutex;
		// END crital section

	};
} // end namespace dls

#include "dls2/msg_wrappers/signal.tpp"

#endif /* end of include guard: SIGNAL_HPP */
