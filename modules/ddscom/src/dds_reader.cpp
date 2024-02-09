
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
			this->reader = this->addReader(partName_, topic_, callback_, qos_);
	}

	DDSReader::~DDSReader(){
		this->reader = nullptr;
	}

} // end namespace dls

#endif /* end of include guard: DDSREADER_CPP */
