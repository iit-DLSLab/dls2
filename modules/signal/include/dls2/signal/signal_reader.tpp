
#ifndef SIGNAL_READER_TPP
#define SIGNAL_READER_TPP

#include "dls2/signal/signal_reader.hpp"

#include <experimental/random>

namespace dls
{
	template <typename SignalType>
	SignalReader<SignalType>::SignalReader(std::shared_ptr<dls::DDSParticipant> participant,
										   const dls::topicType &topic,
										   const std::shared_ptr<SignalType> signal, const std::function<void()> &auxiliary_callback, eprosima::fastdds::dds::DataReaderQos qos)
		: SignalReaderBase(participant, topic),
		  signal_(signal),
		  auxiliary_callback(auxiliary_callback)
	{
		int id = std::experimental::randint(100000, 999999);
		while(dds_participant_->getWriter(dds_participant_->getName() + "::" + std::to_string(id)) != nullptr)
			id = std::experimental::randint(100000, 999999);

		ID_ = dds_participant_->getName() + "::" + std::to_string(id);

		dds_participant_->addReader(ID_,
									topic,
									std::function<void(void *)>{
										[&](void *tuple)
										{
											std::lock_guard<std::mutex> lock(signal_mutex_);
											signal_->loadMsg(tuple);
											received = true;
											this->auxiliary_callback();
										}},
									qos);
		
		listener_ = dds_participant_->getSubListener(ID_);
	}

	template <typename SignalType>
	SignalReader<SignalType>::~SignalReader()
	{
		this->dds_participant_->deleteReader(this->ID_);
	}

	template <typename SignalType>
	SignalType SignalReader<SignalType>::getData()
	{
		return *signal_;
	}

	template <typename SignalType>
	WrapperBase *SignalReader<SignalType>::getWrapperBasePtr()
	{
		return signal_.get();
	}

	template <typename SignalType>
	bool SignalReader<SignalType>::is_receiving_data() const
	{
		return listener_->is_receiving_data();
	}
}
#endif /* end of include guard: SIGNAL_READER_TPP */