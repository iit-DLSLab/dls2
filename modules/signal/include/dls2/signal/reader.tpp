
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
		  auxiliary_callback(auxiliary_callback)
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
}
#endif /* end of include guard: SIGNAL_READER_TPP */