
#ifndef SIGNAL_WRITER_BASE_HPP
#define SIGNAL_WRITER_BASE_HPP

#include "dls2/msg_wrappers/wrapper_base.hpp"
#include "dls2/util/messaging/dds_participant.hpp"

#include <memory>

namespace dls
{
	class SignalWriterBase
	{
	public:
		SignalWriterBase(std::shared_ptr<dls::DDSParticipant> dds_participant);
		~SignalWriterBase();
		SignalWriterBase() = delete;

		virtual void publish() = 0;

		/*!
		@brief Check if the signal stored in any child class
		*/
		virtual bool hasTimestamp() = 0;

		/*!
		@brief Set the timestamp of the signal stored in any child class
		*/
		virtual void setTimestamp(double timestamp) = 0;

	public:
		std::shared_ptr<dls::DDSParticipant> dds_participant_;
		std::string ID_;
		mutable std::mutex signal_mutex_;
		
		/*!
		@brief Set data of the signal stored in the children classes from a wrapper_base pointer.
		It assumes that the pointer points to the same type of the signal
		@param[in] wrapper_base pointer to a wrapper object of the same type of the signal stored in the children classes
		*/
		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base) = 0;
	};
} // end namespace dls

#endif /* end of include guard: SIGNAL_WRITER_BASE_HPP */