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
#ifndef DDSREADER_CPP
#define DDSREADER_CPP

#include "dls2/util/messaging/dds_reader.hpp"

namespace dls
{
	DDSReader::DDSReader(
		std::string     partName_,
		dls::domainType domain_,
		dls::topicType  topic_,
		std::function<void(void *)> callback_,
		eprosima::fastdds::dds::DataReaderQos qos_
	)
		: DDSParticipant(partName_, domain_)
	{
		if (callback_ != nullptr)
			this->reader = this->addReader("unicReader", topic_, callback_, qos_);
	}

	DDSReader::~DDSReader(){
		this->reader = nullptr;
	}

} // end namespace dls

#endif /* end of include guard: DDSREADER_CPP */
