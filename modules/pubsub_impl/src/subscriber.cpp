/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef SUBSCRIBER_CPP
#define SUBSCRIBER_CPP

#include "dls2/util/messaging/participant.hpp"
#include "dls2/util/messaging/subscriber.hpp"

#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/participant/Participant.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/TopicDataType.h>
#include <fastrtps/Domain.h>

#include <stdexcept>
#include <sstream>

/// \cond doxygen_namespace_dls
namespace dls
{
	/// \cond doxygen_namespace_version2
	///
	/// Temporary namespace until the old publishers and subscribers are
	/// refactored into those contained here, then this namespace will be
	/// removed and its contents lifted to the dls namespace
	namespace version2
	{

		Subscriber::Subscriber
		(
			eprosima::fastdds::dds::DomainParticipant *participant_
		) :
			participant(participant_),
			subscriber(nullptr),
			reader(nullptr),
			subscriber_listener(nullptr)
		{
			this->subscriber = this->participant->create_subscriber(
				eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT,
				nullptr
			);

			if(this->subscriber == nullptr){
				throw std::runtime_error(
					"Error: could not create subscriber"
				);
			}
		}

		Subscriber::~Subscriber()
		{
			if(this->subscriber != nullptr){
				this->participant->delete_subscriber(this->subscriber);
			}
		}

		bool Subscriber::addDataReader(
			eprosima::fastdds::dds::Topic	*topic_,
			CallbackType 					callback
		)
		{
			this->reader = this->subscriber->create_datareader(
				topic_,
				eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT,
				&this->subscriber_listener
			);

			if(this->reader == nullptr){
				// throw std::runtime_error(
				// 	"Error: could not create subscriber reader"
				// );
				return false;
			}

			return true;
		}

		
		// =====================================================================
		// Helper Listener Class
		// =====================================================================

		Subscriber::SubListener::SubListener(
			CallbackType callback_) :
			sample_count(0),
			callback(callback_),
			msg()
		{ 
		}

		void Subscriber::SubListener::on_subscription_matched(
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

		void Subscriber::SubListener::on_data_available(
			eprosima::fastdds::dds::DataReader *reader)
		{
			eprosima::fastdds::dds::SampleInfo info;
			if (reader->take_next_sample(&this->msg, &info)	== /*eprosima::fastdds::dds::*/ReturnCode_t::RETCODE_OK)
			{
				if(info.valid_data)
				{
					this->sample_count++;
					this->callback(this->msg);
				}
			}
		}
	} /// \endcond namespace version2
} /// \endcond namespace dls
#endif /* end of include guard: SUBSCRIBER_CPP */
