
#ifndef WRITER_BASE_HPP
#define WRITER_BASE_HPP

#include "dls2/signal/signal_base.hpp"
#include "dls2/util/messaging/dds_participant.hpp"

#include <memory>

namespace dls
{
	class WriterBase : public SignalBase
	{
	public:
		WriterBase(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic);
		~WriterBase();
		WriterBase() = delete;

		virtual void publish() = 0;

		/*!
		@brief Check if the signal msg has a header field
		*/
		virtual bool hasHeader() = 0;

		/*!
		@brief Check if the signal msg has a timestamp field
		*/
		virtual bool hasTimestamp() = 0;

		/*!
		@brief Check if the signal msg has a sequence id field
		*/
		virtual bool hasSequenceId() = 0;

		/*!
		@brief Set the timestamp of the signal stored in any child class
		*/
		virtual void setTimestamp(double timestamp) = 0;

		/*!
		@brief Set the sequence id of the signal stored in any child class
		*/
		virtual void setSequenceId(uint32_t sequence_id) = 0;

		std::vector<std::string> getMatchedReaders();
	};
} // end namespace dls

#endif /* end of include guard: WRITER_BASE_HPP */