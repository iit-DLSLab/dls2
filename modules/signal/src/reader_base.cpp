#include "dls2/signal/reader_base.hpp"

namespace dls
{
	ReaderBase::ReaderBase(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic)
		: SignalBase(dds_participant, topic)
		, received(false)
	{ }

	ReaderBase::~ReaderBase() {}
}