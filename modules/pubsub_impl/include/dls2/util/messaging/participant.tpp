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
#ifndef PARTICIPANT_TPP
#define PARTICIPANT_TPP

#include "dls2/util/messaging/participant.hpp"


/// \cond doxygen_namespace_dls
namespace dls
{

	template <class PubSub_t>
	bool DDSParticipant::addSubscriber(){
		subscribers.emplace_back(
			new dls::version2::Subscriber<PubSub_t>(this->participant)
		);

		return true;
	}

	template <class PubSub_t>
	bool DDSParticipant::addPublisher(){
		publishers.emplace_back(
		 	new dls::version2::Publisher<PubSub_t>(this->participant)
		);

		return true;
	}
	
} // namespace dls
/// \endcond

#endif /* end of include guard: PARTICIPANT_TPP */