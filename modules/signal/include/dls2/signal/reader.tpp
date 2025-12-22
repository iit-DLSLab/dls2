
#ifndef READER_TPP
#define READER_TPP

#include "dls2/signal/reader.hpp"

#include <experimental/random>

namespace dls
{
	template <typename MsgType>
	Reader<MsgType>::Reader(std::shared_ptr<dls::DDSParticipant> participant,
										   const dls::topicType &topic,
										   const std::function<void()> &auxiliary_callback, eprosima::fastdds::dds::DataReaderQos qos)
		: ReaderBase(participant, topic),
		  auxiliary_callback(auxiliary_callback),
		  has_header_(HasHeader<MsgType>::value),
		  has_sequence_id_(HasSequenceId<MsgType>::value)
	{
		computeName("reader");

		dds_participant_->addReader(ID_,
									topic,
									std::function<void(void *)>{
										[&](void *tuple)
										{
											// Suppress unused parameter warning
											static_cast<void>(tuple);

											received = true;
											this->auxiliary_callback();
										}},
									qos);
		
		listener_ = dds_participant_->getSubListener(ID_);
	}

	template <typename MsgType>
	Reader<MsgType>::~Reader()
	{
		this->dds_participant_->deleteReader(this->ID_);
	}

	template <typename MsgType>
	bool Reader<MsgType>::is_receiving_data() const
	{
		return listener_->is_receiving_data();
	}

	template <typename MsgType>
	int Reader<MsgType>::getRelativeSampleCount(){
		int count = listener_->relative_sample_count;
		listener_->resetRelativeSampleCount();
		return count;
	}

	template <typename MsgType>
	bool Reader<MsgType>::hasStartedReceivingData(){
		return listener_->started_receiving_data_;
	}

	template <typename MsgType>
	int Reader<MsgType>::sampleCount(){
		int count = listener_->sample_count;
		return count;
	}

	template <typename MsgType>
	void Reader<MsgType>::read()
	{
		if(listener_ == nullptr)
		{
			std::cout << "Listener of " << ID_<<" is null, cannot read data" << std::endl;
			return;
		}

		std::shared_lock<std::shared_mutex> lock(listener_->listener_info_mtx);

		if(is_receiving_data()){
			this->msg = *static_cast<MsgType*>(listener_->msg);
		}
	}

	template <typename MsgType>
	std::chrono::steady_clock::time_point Reader<MsgType>::get_latest_timestamp()
	{
		return listener_->last_timestamp;
	}

	template <typename MsgType>
	double Reader<MsgType>::get_latest_period_ms()
	{
		return listener_->last_period_ms;
	}

	template <typename MsgType>
	bool Reader<MsgType>::hasHeader()
	{
		return has_header_;
	}

	template <typename MsgType>
	bool Reader<MsgType>::hasSequenceId()
	{
		return has_header_ || has_sequence_id_;
	}

	template <typename MsgType>
	uint32_t Reader<MsgType>::getLatestSequenceId()
	{
		if constexpr (HasHeader<MsgType>::value){
			return this->msg.header().sequence_id();
		}else if constexpr (HasSequenceId<MsgType>::value){
			return this->msg.sequence_id();
		}else{
			throw std::runtime_error("Calling getLatestSequenceId for msg type " + *typeid(MsgType).name());
		}
	}
}
#endif /* end of include guard: SIGNAL_READER_TPP */