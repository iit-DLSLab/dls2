
#ifndef SIGNAL_READER_BASE_HPP
#define SIGNAL_READER_BASE_HPP

#include "dls2/msg_wrappers/wrapper_base.hpp"
#include "dls2/util/messaging/dds_participant.hpp"

#include <memory>

namespace dls
{
	class SignalReaderBase
	{
	public:
		friend class Plugin;
		SignalReaderBase(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic);
		~SignalReaderBase();
		SignalReaderBase() = delete;	
		bool received;

		virtual bool is_receiving_data() const = 0;
		dls::topicType getTopic() const;

	protected:
		std::shared_ptr<dls::DDSParticipant> dds_participant_;
		std::string ID_;
		mutable std::mutex signal_mutex_;
		dls::topicType topic_;

		/*!
		@brief Return a WrapperBase pointer, pointing to the signal (of Wrapper class) stored in the children classes of this class
		*/
		virtual WrapperBase* getWrapperBasePtr() = 0;
	};
} // end namespace dls

#endif /* end of include guard: SIGNAL_READER_BASE_HPP */