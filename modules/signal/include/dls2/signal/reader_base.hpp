
#ifndef READER_BASE_HPP
#define READER_BASE_HPP

#include "dls2/signal/signal_base.hpp"
#include "dls2/util/messaging/dds_participant.hpp"

#include <memory>

namespace dls
{
	class ReaderBase : public SignalBase
	{
	public:
		ReaderBase(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic);
		~ReaderBase();
		ReaderBase() = delete;	
		
		virtual bool is_receiving_data() const = 0;
		virtual int getRelativeSampleCount() = 0;
		virtual bool hasStartedReceivingData() = 0;
		virtual bool hasHeader() = 0;
		virtual bool hasSequenceId() = 0;
		virtual int sampleCount() = 0;

		virtual void read() = 0;
		virtual std::chrono::steady_clock::time_point get_latest_timestamp() = 0;
		virtual double get_latest_period_ms() = 0;

		virtual uint32_t getLatestSequenceId() = 0;

		bool received;
	};
} // end namespace dls

#endif /* end of include guard: SIGNAL_READER_BASE_HPP */