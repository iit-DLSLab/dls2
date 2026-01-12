#ifndef WRITER_TPP
#define WRITER_TPP

#include "dls2/signal/writer.hpp"
#include "dls2/util/time/duration_utils.hpp"

#include <experimental/random>

using namespace dls;

template <typename MsgType>
Writer<MsgType>::Writer(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic_, eprosima::fastdds::dds::DataWriterQos qos)
	: WriterBase(dds_participant, topic_)
	, has_header_(HasHeader<MsgType>::value)
	, has_timestamp_(HasTimeStamp<MsgType>::value)
	, has_sequence_id_(HasSequenceId<MsgType>::value)
{
	computeName("writer");
	dds_participant_->addWriter(ID_, topic_, qos);
	if(hasTimestamp()){
		setTimestamp(0.0);
	}
}
	
template <typename MsgType>
Writer<MsgType>::~Writer(){this->dds_participant_->deleteWriter(this->ID_); }

template <typename MsgType>
void Writer<MsgType>::publish()
{
	if (hasTimestamp() && isSameTimestamp())
	{
		setTimestamp(toNs<unsigned long long>(std::chrono::system_clock::now()));
	}
	if (hasSequenceId())
	{
		setSequenceId(++sequence_id_ % MAX_SEQUENCE_ID);
	}

	dds_participant_->sendMessage(ID_, &msg);
}

template <typename MsgType>
bool Writer<MsgType>::hasHeader()
{
	return has_header_;
}

template <typename MsgType>
bool Writer<MsgType>::hasTimestamp()
{
	return has_header_ || has_timestamp_;
}

template <typename MsgType>
bool Writer<MsgType>::hasSequenceId()
{
	return has_header_ || has_sequence_id_;
}

template <typename MsgType>
void Writer<MsgType>::setTimestamp(unsigned long long timestamp)
{
	if constexpr (HasHeader<MsgType>::value)
	{
		msg.header().timestamp() = timestamp;
		prev_stamp = msg.header().timestamp();

	}else if constexpr (HasTimeStamp<MsgType>::value)
	{
		msg.timestamp() = timestamp;
		prev_stamp = msg.timestamp();

	}else{
		std::cout << "WARNING: Calling setTimestamp for msg type " + std::string(typeid(MsgType).name());
	}
}

template <typename MsgType>
bool Writer<MsgType>::isSameTimestamp()
{
	unsigned long long curr_stamp;

	if constexpr (HasHeader<MsgType>::value)
	{
		curr_stamp = msg.header().timestamp();

	}else if constexpr (HasTimeStamp<MsgType>::value)
	{
		curr_stamp = msg.timestamp();
	}else{
		std::cout << "WARNING: Calling sameTimestamp for msg type " + std::string(typeid(MsgType).name());
	}

	if(prev_stamp == 0 && curr_stamp == 0){
		// first execution, nobody set timestamp
		return true;
	}
	else if(prev_stamp == 0 && curr_stamp != 0){
		// first execution, somebody set timestamp
		prev_stamp = curr_stamp;
		return false;
	}
	// standard execution, checking if timestamp is same as previous
	auto same_stamp = (prev_stamp == curr_stamp);
	if(!same_stamp){
		prev_stamp = curr_stamp;
	}
	return same_stamp;
}

template <typename MsgType>
void Writer<MsgType>::setSequenceId(uint32_t sequence_id)
{
	if constexpr (HasHeader<MsgType>::value){
		msg.header().sequence_id() = sequence_id;
	}else if constexpr (HasSequenceId<MsgType>::value){
		msg.sequence_id() = sequence_id;
	}else{
		std::cout << "WARNING: Calling setSequenceId for msg type " + std::string(typeid(MsgType).name());
	}
}

#endif /* end of include guard: WRITER_TPP */