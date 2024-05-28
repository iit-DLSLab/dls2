#include "dls2/msg_wrappers/signal_reader_base.hpp"

namespace dls
{
	SignalReaderBase::SignalReaderBase(std::shared_ptr<dls::DDSParticipant> dds_participant)
		: received(false)
		, dds_participant_(dds_participant)
		, ID_("")
	{ }

	SignalReaderBase::~SignalReaderBase(){}
}