#include "dls2/msg_wrappers/signal_writer_base.hpp"

namespace dls
{
	SignalWriterBase::SignalWriterBase(std::shared_ptr<dls::DDSParticipant> dds_participant)
		: dds_participant_(dds_participant)
		, ID_("")
	{ }

	SignalWriterBase::~SignalWriterBase(){}
}