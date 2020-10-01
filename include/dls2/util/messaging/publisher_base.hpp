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
#ifndef PUBLISHER_BASE_HPP_MFE9PIJK
#define PUBLISHER_BASE_HPP_MFE9PIJK

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
		eprosima::fastrtps::rtps::GUID_t getGuid() const;

		// TODO temp, remove
		const std::string temp_topic;
	};
} // end namespace dls

// =============================================================================
// New version
// =============================================================================
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
		template <class PubSub_t>
		class Publisher
		{
			// namespace dds = eprosima::fastdds::dds;
			template <typename T, typename U> friend class Service;
			template <typename T, typename U> friend class ServiceClient;
		public:
			Publisher(const std::string &topic);
			virtual ~Publisher();

			void publish(typename PubSub_t::type &msg) const;

		private:
			// eprosima::fastdds::dds::DomainParticipant *participant;
			eprosima::fastdds::dds::Publisher         *publisher;
			eprosima::fastdds::dds::Topic             *topic;
			eprosima::fastdds::dds::DataWriter        *writer;
			eprosima::fastdds::dds::TypeSupport       type;

			class PublisherListener :
				public eprosima::fastdds::dds::DataWriterListener
			{
			public:
				PublisherListener();
				void on_publication_matched
				(
					eprosima::fastdds::dds::DataWriter *,
					const eprosima::fastdds::dds::PublicationMatchedStatus &info
				) override;

				std::atomic_int matched_count;
			} publisher_listener;

			PubSub_t rtps_type;
		};
	} /// \endcond namespace version2
} /// \endcond namespace dls

#include "dls2/util/messaging/publisher_base.tpp"

#endif /* end of include guard: PUBLISHER_BASE_HPP_MFE9PIJK */
