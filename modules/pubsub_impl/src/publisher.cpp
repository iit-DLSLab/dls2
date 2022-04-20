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
#ifndef PUBLISHER_CPP
#define PUBLISHER_CPP

#include "dls2/util/messaging/participant.hpp"
#include "dls2/util/messaging/publisher.hpp"

#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/participant/Participant.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/TopicDataType.h>
#include <fastrtps/Domain.h>

#include <stdexcept>

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
		Publisher::Publisher(
			eprosima::fastdds::dds::DomainParticipant *participant_
		) :
			participant(participant_),
			publisher(nullptr),
			writer(nullptr)
		{
			this->publisher = this->participant->create_publisher(
				eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT,
				nullptr
			);

			if(this->publisher == nullptr){
				throw std::runtime_error(
					"Error: could not create publisher"
				);
			}
		}

		Publisher::~Publisher()
		{		
			if(this->publisher != nullptr){
				this->participant->delete_publisher(this->publisher);
			}	
		}

		bool Publisher::addDataWriter(
			eprosima::fastdds::dds::Topic 	*topic_
		)
		{
			this->writer = this->publisher->create_datawriter(
				topic_,
				eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT,
				&this->publisher_listener
			);

			if(this->writer == nullptr)	{
				// throw std::runtime_error(
				// 	"Error: could not create publisher writer"
				// );
				return false;
			}

			return true;
		}

		void Publisher::publish(void *msg) const{
			if(publisher_listener.matched_count > 0){
				this->writer->write(msg);
			}
		}

		auto Publisher::getGuid() const -> eprosima::fastrtps::rtps::GUID_t
		{
			std::vector<eprosima::fastdds::dds::DataWriter*> writers;
			this->publisher->get_datawriters(writers);
			assert(!writers.empty());
			return writers[0]->guid();
		}

		// =====================================================================
		// Helper Listener Class
		// =====================================================================
		Publisher::PublisherListener::PublisherListener()
		{ }

		void Publisher::PublisherListener::on_publication_matched
		(
			eprosima::fastdds::dds::DataWriter*,
			const eprosima::fastdds::dds::PublicationMatchedStatus &info
		)
		{
			if(info.current_count_change == 1){
				// publisher matched
				this->matched_count = info.total_count;
			}
			else if(info.current_count_change == -1){
				// publisher unmatched
				this->matched_count = info.total_count;
			}
			else{
				// invalid
			}
		}
	} /// \endcond namespace version2
} /// \endcond namespace dls
#endif /* end of include guard: PUBLISHER_CPP */
