#include "dls2/signal/signal_reader_base.hpp"

namespace dls
{
	SignalReaderBase::SignalReaderBase(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic)
		: received(false)
		, dds_participant_(dds_participant)
		, ID_("")
		, topic_(topic)
	{ }

	SignalReaderBase::~SignalReaderBase(){}

	dls::topicType SignalReaderBase::getTopic() const
	{
		return topic_;
	}
}