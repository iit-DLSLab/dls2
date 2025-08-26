
#ifndef SIGNAL_BASE_HPP
#define SIGNAL_BASE_HPP

#include "dls2/util/messaging/dds_participant.hpp"

#include <memory>

namespace dls
{
	class SignalBase
	{
	public:

		SignalBase(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic);
		~SignalBase();
		SignalBase() = delete;	

		dls::topicType getTopic() const;

		std::string getID() const;

	protected:
		std::shared_ptr<dls::DDSParticipant> dds_participant_;
		std::string ID_;
		mutable std::mutex signal_mutex_;
		dls::topicType topic_;
		
		void computeName(const std::string& prefix);
	};
} // end namespace dls

#endif /* end of include guard: SIGNAL_READER_BASE_HPP */