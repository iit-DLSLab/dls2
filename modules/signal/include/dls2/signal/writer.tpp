#ifndef WRITER_TPP
#define WRITER_TPP

#include "dls2/signal/writer.hpp"

#include <experimental/random>

using namespace dls;

template <typename MsgType>
Writer<MsgType>::Writer(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic_, eprosima::fastdds::dds::DataWriterQos qos)
	: WriterBase(dds_participant, topic_)
	, has_timestamp_(HasTimeStamp<MsgType>::value)
{
	computeName("writer");
	dds_participant_->addWriter(ID_, topic_, qos);
}
	
template <typename MsgType>
Writer<MsgType>::~Writer(){this->dds_participant_->deleteWriter(this->ID_); }

template <typename MsgType>
void Writer<MsgType>::publish()
{
	dds_participant_->sendMessage(ID_, &msg);
}

template <typename MsgType>
bool Writer<MsgType>::hasTimestamp()
{
	return has_timestamp_;
}

template <typename MsgType>
void Writer<MsgType>::setTimestamp(double timestamp)
{
	msg.timestamp() = timestamp;
}

#endif /* end of include guard: WRITER_TPP */