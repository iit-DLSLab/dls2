
#ifndef SIGNAL_WRITER_TPP
#define SIGNAL_WRITER_TPP

#include "dls2/signal/signal_writer.hpp"

#include <experimental/random>

using namespace dls;

template <typename SignalType>
SignalWriter<SignalType>::SignalWriter(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic_, const std::shared_ptr<SignalType> signal, eprosima::fastdds::dds::DataWriterQos qos)
	: SignalWriterBase(dds_participant, topic_)
	, signal_(signal)
	, has_header_(HasHeader<SignalType>::value)
	, has_timestamp_(HasTimeStamp<SignalType>::value)
{
	int id = std::experimental::randint(100000, 999999);
	while(dds_participant_->getWriter(dds_participant_->getName() + "::" + std::to_string(id)) != nullptr)
		id = std::experimental::randint(100000, 999999);

	ID_ = dds_participant_->getName() + "::" + std::to_string(id);

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
    // std::cout << "before sendMessage\n";
	dds_participant_->sendMessage(ID_, signal_->getMsg());
}

template <typename SignalType>
bool SignalWriter<SignalType>::hasHeader()
{
	return has_header_;
}

template <typename SignalType>
bool SignalWriter<SignalType>::hasTimestamp()
{
	return has_header_ || has_timestamp_;
}

template <typename SignalType>
void SignalWriter<SignalType>::setTimestamp(double timestamp)
{
	if constexpr (HasHeader<MsgType>::value){
		signal_->header.timestamp = timestamp;
	}else if constexpr (HasTimeStamp<MsgType>::value){
		signal_->timestamp = timestamp;
	}else{
		throw std::runtime_error("Calling setTimestamp for msg type " + *typeid(MsgType).name());
	}
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