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
#ifndef PUBLISHER_HPP_MFE9PIJK
#define PUBLISHER_HPP_MFE9PIJK

// =============================================================================
// Old Includes .. To be removed
// =============================================================================
#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/publisher/PublisherListener.h>

// =============================================================================
// New includes
// =============================================================================
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include <string>

// =============================================================================
// Old Version -- to be removed
// =============================================================================
namespace dls
{
	template <class PubSub_t>
	class PublisherBase : public eprosima::fastrtps::PublisherListener
	{
		template <typename T, typename U> friend class Service;
		template <typename T, typename U> friend class ServiceClient;
	public:
		PublisherBase(const std::string &topic);
		virtual ~PublisherBase();

		void publish(typename PubSub_t::type &msg) const;

	private:
		eprosima::fastrtps::Participant *pParticipant;
		eprosima::fastrtps::Publisher *pPublisher;

		/*static*/ PubSub_t rtps_type;

		/// Returns the underlying fastrtps guid of this publisher
		///
		/// This should _not_ be exposed to third party clients. This is used
		/// only for identification of a specific publisher in the Service and
		/// ServiceClient implementations. This can be removed without warning
		auto getGuid() const -> eprosima::fastrtps::rtps::GUID_t;

		// TODO temp, remove
		const std::string temp_topic;

	};
} // end namespace dls

#include "dls2/util/messaging/publisher.tpp"

#endif /* end of include guard: PUBLISHER_HPP_MFE9PIJK */
