
// TODO this is a tool that needs to be made into a separate project and made
// more sophisticated
#ifndef TOPIC_INFO_HPP_SVMAPUJA
#define TOPIC_INFO_HPP_SVMAPUJA

#include <chrono>
#include <fastrtps/participant/ParticipantListener.h>
#include <string>
#include <mutex>
#include <map>
#include <memory>
#include "dls2/util/messaging/subscriber.hpp"
#include "dls_messages/dds/stringmsgPubSubTypes.h"
#include <atomic>

namespace dls
{
	class TopicInfo;
}
std::ostream &operator<<(std::ostream &os, const dls::TopicInfo&);

namespace dls
{
	class TopicInfo : public eprosima::fastrtps::ParticipantListener
	{
		friend std::ostream &::operator<<(std::ostream &os, const TopicInfo&);
	public:
		TopicInfo();

		std::string getTopicType(const std::string &topic);
		void echo(const std::string &topic);
		void hz(const std::string &topic);

	private:
		void onSubscriberDiscovery
		(
			eprosima::fastrtps::Participant *participant,
			eprosima::fastrtps::rtps::ReaderDiscoveryInfo &&info
		) override;

		void onPublisherDiscovery
		(
			eprosima::fastrtps::Participant *participant,
			eprosima::fastrtps::rtps::WriterDiscoveryInfo &&info
		) override;

		// Begin critical section
			mutable std::mutex topics_publishers_mutex;
			std::multimap<std::string, eprosima::fastrtps::rtps::WriterDiscoveryInfo> writer_info;
		// End critical section

		// Begin critical section
			mutable std::mutex topics_subscribers_mutex;
			std::multimap<std::string, eprosima::fastrtps::rtps::ReaderDiscoveryInfo> reader_info;
		// End critical section

		// class HzSub : public SubscriberBase<StringMsgPubSubType>
		class HzSub : public eprosima::fastrtps::SubscriberListener
		{
			friend class TopicInfo;
		public:
			HzSub(const std::string &topic);
		private:
			void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;
			std::atomic<size_t> msg_count;

		};

		// std::shared_ptr<HzSub> pHz_sub;
	};
} // namespace dls
#endif /* end of include guard: TOPIC_INFO_HPP_SVMAPUJA */
