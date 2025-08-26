#include "dls2/signal/signal_base.hpp"

#include <experimental/random>

namespace dls
{
	SignalBase::SignalBase(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic)
		: dds_participant_(dds_participant)
		, ID_("")
		, topic_(topic)
	{ }

	SignalBase::~SignalBase(){}

	dls::topicType SignalBase::getTopic() const
	{
		return topic_;
	}

	void SignalBase::computeName(const std::string& prefix)
	{
		int id = std::experimental::randint(100000, 999999);
		ID_ = dds_participant_->getName() + "::" + prefix + "_"+std::to_string(id);
		while(dds_participant_->getReader(ID_) != nullptr){
			id = std::experimental::randint(100000, 999999);
			ID_ = dds_participant_->getName() + "::" + prefix + "_"+std::to_string(id);
		}
	}

	std::string SignalBase::getID() const
	{
		return ID_;
	}
}