
#ifndef SIGNAL_WRITER_TPP
#define SIGNAL_WRITER_TPP

#include "dls2/msg_wrappers/signal_writer.hpp"

#include <experimental/random>

using namespace dls;

template <typename SignalType>
SignalWriter<SignalType>::SignalWriter(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic_, const std::shared_ptr<SignalType> signal, eprosima::fastdds::dds::DataWriterQos qos)
	: SignalWriterBase(dds_participant)
	, signal_(signal)
	, has_timestamp_(HasTimeStamp<SignalType>::value)
{
	int id = std::experimental::randint(100000, 999999);
	while(dds_participant_->getWriter(std::to_string(id)) != nullptr)
		id = std::experimental::randint(100000, 999999);

	ID_ = std::to_string(id);

	dds_participant_->addWriter(ID_, topic_, qos);
}
	
template <typename SignalType>
SignalWriter<SignalType>::~SignalWriter(){this->dds_participant_->deleteWriter(this->ID_); }

template <typename SignalType>
std::shared_ptr<SignalType> SignalWriter<SignalType>::operator->() 
{
	std::lock_guard<std::mutex> lock(signal_mutex_);
    return signal_;
}

template <typename SignalType>
void SignalWriter<SignalType>::publish()
{
	std::lock_guard<std::mutex> lock(signal_mutex_);
	dds_participant_->sendMessage(ID_, signal_->getMsg());
}

template <typename SignalType>
bool SignalWriter<SignalType>::hasTimestamp()
{
	return has_timestamp_;
}

template <typename SignalType>
void SignalWriter<SignalType>::setTimestamp(double timestamp)
{
	signal_->timestamp_ = timestamp;
}

template <typename SignalType>
void SignalWriter<SignalType>::setSignal(const SignalType& signal)
{
	*signal_ = signal;
}

template <typename SignalType>
void SignalWriter<SignalType>::setDataFromWrapperBase(WrapperBase* wrapper_base)
{
	signal_->setDataFromWrapperBase(wrapper_base);
}

#endif /* end of include guard: SIGNAL_WRITER_TPP */