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
#ifndef DDS_READER_HPP
#define DDS_READER_HPP

#include "dls2/util/messaging/participant.hpp"

namespace dls
{
	template <class PubSub_t>
	class DDSReader :: public DDSParticipant
	{
	public:
		DDSReader(
			std::string     partName_,
			dls::domainType domain_,
			dls::topicType  topic_
		);
		virtual ~DDSReader();

	private:
		
	};
}

#include "dls2/util/messaging/dds_reader.tpp"

#endif /* end of include guard: SUBSCRIBER_HPP_XPACOJJI */
