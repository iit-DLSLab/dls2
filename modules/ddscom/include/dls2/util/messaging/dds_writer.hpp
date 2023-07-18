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
#ifndef DDSWRITER_HPP
#define DDSWRITER_HPP

#include "dls2/util/messaging/dds_participant.hpp"


namespace dls
{
	class DDSWriter : public DDSParticipant
	{
	public:

		DDSWriter(
			std::string     			partName_,
			dls::domainType 			domain_,
			dls::topicType  			topic_,
			eprosima::fastdds::dds::DataWriterQos qos_ = eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT
		);

		virtual ~DDSWriter();

		void sendMessage(void *msg);

	private:

		eprosima::fastdds::dds::DataWriter *writer;

	};
}

#endif /* end of include guard: WRITER_HPP */
