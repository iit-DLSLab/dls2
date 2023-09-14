
#ifndef DDSWRITER_CPP
#define DDSWRITER_CPP

#include "dls2/util/messaging/dds_writer.hpp"

namespace dls
{
	DDSWriter::DDSWriter(
		std::string     partName_,
		dls::domainType domain_,
		dls::topicType  topic_,
		eprosima::fastdds::dds::DataWriterQos qos_
	)
		: DDSParticipant(partName_, domain_)
	{
		this->writer = this->addWriter(partName_, topic_, qos_);
	}

	DDSWriter::~DDSWriter(){}

	void DDSWriter::sendMessage(void *msg){
		this->writer->write(msg);
	}

} // end namespace dls

#endif /* end of include guard: DDSWRITER_CPP */
