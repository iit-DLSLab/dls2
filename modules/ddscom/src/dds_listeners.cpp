
#ifndef DDSLISTENERS_CPP
#define DDSLISTENERS_CPP

#include "dls2/util/messaging/dds_listeners.hpp"


/// \cond doxygen_namespace_dls
namespace dls
{
	// =====================================================================
	// Publisher Helper Listener Class Implementation
	// =====================================================================

	DDSPubListener::DDSPubListener()
	{ }

	void DDSPubListener::on_publication_matched
	(
		eprosima::fastdds::dds::DataWriter*,
		const eprosima::fastdds::dds::PublicationMatchedStatus &info
	)
	{
		if(info.current_count_change == 1){
			// publisher matched
			this->matched_count = info.current_count;
			matched_datareaders_instances.push_back(info.last_subscription_handle);
		}
		else if(info.current_count_change == -1){
			// publisher unmatched
			this->matched_count = info.current_count;
			auto position(std::find(matched_datareaders_instances.begin(), matched_datareaders_instances.end(), info.last_subscription_handle));
			if(position!=matched_datareaders_instances.end())
				matched_datareaders_instances.erase(position);
		}
		else{
			// invalid
		}
	}

	// =====================================================================
	// Subscriber Helper Listener Class Implementation
	// =====================================================================

	DDSSubListener::DDSSubListener(
		std::function<void(void *)> callback_) 
		: sample_count(0)
		, callback(callback_)
		, msg(nullptr)
	{ 
	}

	DDSSubListener::~DDSSubListener(){
		free(this->msg);
	}

	void DDSSubListener::on_subscription_matched(
		eprosima::fastdds::dds::DataReader*,
		const eprosima::fastdds::dds::SubscriptionMatchedStatus &info)
	{
		if(info.current_count_change == 1)
		{
			// subscriber matched
		}
		else if(info.current_count_change == -1)
		{
			// subscriber unmatched
		}
		else
		{
			// invalid
		}
	}


	void DDSSubListener::on_data_available(
		eprosima::fastdds::dds::DataReader *reader)
	{
		eprosima::fastdds::dds::SampleInfo info;

		if (this->msg == nullptr)
			this->msg = reader->type().create_data();

		if (reader->take_next_sample(this->msg, &info)	== ReturnCode_t::RETCODE_OK)
		{
			if(info.valid_data)
			{
				this->sample_count++;
				this->callback(this->msg);
			}
		}
	}
} /// \endcond namespace dls
#endif /* end of include guard: DDSLISTENERS_CPP */
