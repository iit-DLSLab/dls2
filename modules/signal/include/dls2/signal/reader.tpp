
#ifndef READER_TPP
#define READER_TPP

#include "dls2/signal/reader.hpp"

#include <experimental/random>
#include <stdexcept>

namespace dls
{
	template <typename MsgType>
	Reader<MsgType>::Reader(std::shared_ptr<dls::DDSParticipant> participant,
										   const dls::topicType &topic,
										   const std::function<void()> &auxiliary_callback, eprosima::fastdds::dds::DataReaderQos qos)
		: Reader(participant, topic, auxiliary_callback, qos, {})
	{ }

	template <typename MsgType>
	Reader<MsgType>::Reader(std::shared_ptr<dls::DDSParticipant> participant,
		const dls::topicType &topic,
		const std::function<void(const MsgType&)> &sample_callback,
		eprosima::fastdds::dds::DataReaderQos qos)
		: Reader(participant, topic, [](){}, qos, sample_callback)
	{ }

	template <typename MsgType>
	Reader<MsgType>::Reader(std::shared_ptr<dls::DDSParticipant> participant,
		const dls::topicType &topic, const std::function<void()> &auxiliary_callback,
		eprosima::fastdds::dds::DataReaderQos qos,
		const std::function<void(const MsgType&)> &sample_callback)
		: ReaderBase(participant, topic),
		  auxiliary_callback(auxiliary_callback),
		  sample_callback_(sample_callback),
		  has_header_(HasHeader<MsgType>::value),
		  has_sequence_id_(HasSequenceId<MsgType>::value)
	{
		computeName("reader");

		// Publish listener_ before create_datareader can invoke its callbacks.
		listener_ = std::make_shared<DDSSubListener>(
			std::function<void(void *)>{
										[this](void *tuple)
										{
											// Suppress unused parameter warning
											static_cast<void>(tuple);

											if constexpr (HasSequenceId<MsgType>::value || HasHeader<MsgType>::value)
											{
												// Read current sequence id
												const MsgType* msg = static_cast<MsgType*>(tuple);
												long current_seq_id = 0;
												if constexpr (HasSequenceId<MsgType>::value)
												{
													current_seq_id = static_cast<long>(msg->sequence_id());
												}else if constexpr (HasHeader<MsgType>::value)
												{
													current_seq_id = static_cast<long>(msg->header().sequence_id());
												}

												// Check sequence id sanity
												if(this->listener_->sample_count > 1)
												{
													auto expected_seq_id = static_cast<long>((this->prev_sequence_id_ + 1) % MAX_SEQUENCE_ID);
													this->missed_sequence_ids_ = std::abs(expected_seq_id - current_seq_id);
												}else
												{
													this->missed_sequence_ids_ = 0;
												}
												this->prev_sequence_id_ = current_seq_id;
											}
											
											received = true;
											if (this->sample_callback_) {
												this->sample_callback_(*static_cast<const MsgType*>(tuple));
											}
											this->auxiliary_callback();
										}});

		auto reader = dds_participant_->addReader(ID_, topic, listener_, qos);
		if (reader == nullptr) {
			throw std::runtime_error("Could not create DDS reader " + ID_);
		}

		const eprosima::fastdds::dds::TopicDescription* topic_desc = reader->get_topicdescription();
		if (topic_desc != nullptr) {
			topic_name_ = topic_desc->get_name();
		}
		
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
	uint32_t Reader<MsgType>::getMissedSequenceIds()
	{
		return missed_sequence_ids_;
	}

	template <typename MsgType>
	std::unordered_map<std::string, std::string> Reader<MsgType>::getTopicToWriter()
	{
		return this->dds_participant_->getTopicToWriter();
	}
}
#endif /* end of include guard: SIGNAL_READER_TPP */