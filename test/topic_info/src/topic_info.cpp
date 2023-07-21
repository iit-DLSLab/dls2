
// TODO this is a tool that needs to be made into a separate project and made
// more sophisticated
#include "dls2/util/topic_info/topic_info.hpp"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>

// TODO temporarily list all the types here
#include "dls_messages/dds/blind_statePubSubTypes.h"
#include "dls_messages/dds/console_commandPubSubTypes.h"
#include "dls_messages/dds/control_signalPubSubTypes.h"
#include "dls_messages/dds/desired_torquesPubSubTypes.h"
#include "dls_messages/dds/gait_signalPubSubTypes.h"
#include "dls_messages/dds/headerPubSubTypes.h"
#include "dls_messages/dds/hello_worldPubSubTypes.h"
#include "dls_messages/dds/imuPubSubTypes.h"
#include "dls_messages/dds/joint_statePubSubTypes.h"
#include "dls_messages/dds/posePubSubTypes.h"
#include "dls_messages/dds/screwPubSubTypes.h"
#include "dls_messages/dds/stringmsgPubSubTypes.h"
#include "dls_messages/dds/timePubSubTypes.h"
#include "dls_messages/dds/vec3PubSubTypes.h"



using namespace dls;

// =============================================================================
// Constructors
// =============================================================================
TopicInfo::TopicInfo() :
	topics_publishers_mutex(),
	writer_info(),
	topics_subscribers_mutex(),
	reader_info()
	// pHz_sub(nullptr)
{

	eprosima::fastrtps::ParticipantAttributes participant_attr;
	participant_attr.rtps.setName("Participant_publisher");
	auto custom_transport = std::make_shared<eprosima::fastrtps::rtps::UDPv4TransportDescriptor>();
	custom_transport->interfaceWhiteList.emplace_back("127.0.0.1");
	participant_attr.rtps.useBuiltinTransports = false;
	participant_attr.rtps.userTransports.push_back(custom_transport);

	eprosima::fastrtps::Domain::createParticipant(participant_attr, this);
}

// =============================================================================
// Interface Override
// =============================================================================
void TopicInfo::onSubscriberDiscovery
(
	eprosima::fastrtps::Participant *participant,
	eprosima::fastrtps::rtps::ReaderDiscoveryInfo &&info
)
{
	(void)participant;
	switch(info.status)
	{
		case eprosima::fastrtps::rtps::ReaderDiscoveryInfo::DISCOVERED_READER:
			{
				std::lock_guard<std::mutex> lock(this->topics_subscribers_mutex);
				this->reader_info.insert
				(
					std::pair<std::string, eprosima::fastrtps::rtps::ReaderDiscoveryInfo>
					(
						std::string(info.info.topicName()),
						info
					)
				);
			}
			break;
		case eprosima::fastrtps::rtps::ReaderDiscoveryInfo::CHANGED_QOS_READER:
			break;
		case eprosima::fastrtps::rtps::ReaderDiscoveryInfo::REMOVED_READER:
			{
				std::lock_guard<std::mutex> lock(this->topics_subscribers_mutex);

				// erase only a signle element in the multimap
				auto it = this->reader_info.find(std::string(info.info.topicName()));
				if(it != this->reader_info.end())
				{
					this->reader_info.erase(it);
				}
			}
			break;

	}
}
void TopicInfo::onPublisherDiscovery
(
	eprosima::fastrtps::Participant *participant,
	eprosima::fastrtps::rtps::WriterDiscoveryInfo &&info
)
{
	(void)participant;
	switch(info.status)
	{
		case eprosima::fastrtps::rtps::WriterDiscoveryInfo::DISCOVERED_WRITER:
			{
				std::lock_guard<std::mutex> lock(this->topics_publishers_mutex);
				this->writer_info.insert
				(
					std::pair<std::string, eprosima::fastrtps::rtps::WriterDiscoveryInfo>
					(
						std::string(info.info.topicName()),
						info
					)
				);
			}
			break;
		case eprosima::fastrtps::rtps::WriterDiscoveryInfo::CHANGED_QOS_WRITER:
			break;
		case eprosima::fastrtps::rtps::WriterDiscoveryInfo::REMOVED_WRITER:
			{
				std::lock_guard<std::mutex> lock(this->topics_publishers_mutex);

				// erase only a single element in the multimap
				auto it = this->writer_info.find(std::string(info.info.topicName()));
				if(it != this->writer_info.end())
				{
					this->writer_info.erase(it);
				}
			}
			break;
	}
}

// =============================================================================
// Implementation
// =============================================================================
std::string TopicInfo::getTopicType(const std::string &topic)
{
	{
		auto it = this->writer_info.find(topic);
		if(it != this->writer_info.end())
		{
			return std::string(it->second.info.typeName());
		}
	}
	{
		auto it = this->reader_info.find(topic);
		if(it != this->reader_info.end())
		{
			return std::string(it->second.info.typeName());
		}
	}

	return std::string("topic: ") + topic + " not found";
}

std::ostream &operator<<(std::ostream &os, const TopicInfo &list)
{
	std::lock_guard<std::mutex> lock1(list.topics_publishers_mutex);
	std::lock_guard<std::mutex> lock2(list.topics_subscribers_mutex);

	if(list.writer_info.size() > 0)
	{
		os << "Publishers Found on Topics:\n";
		for(const auto el: list.writer_info)
		{
			os << "\t" << el.first << "\n";
		}
	}

	if(list.reader_info.size() > 0)
	{
		os << "Subscribers Found on Topics:\n";
		for(const auto el: list.reader_info)
		{
			os << "\t" << el.first << "\n";
		}
	}

	return os;
}

void TopicInfo::echo(const std::string &topic)
{
	std::lock_guard<std::mutex> lock(this->topics_publishers_mutex);
	auto it = this->writer_info.find(topic);
	if(it != this->writer_info.end())
	{
		// std::cout << it->second.info.type_information() << std::endl;
	}
}

void TopicInfo::hz(const std::string &topic)
{
	// this->pHz_sub = std::make_shared<TopicInfo::HzSub>(topic);
	// std::cout << "constructed subscriber on topic '" << topic << "'" << std::endl;
	std::lock_guard<std::mutex> lock(topics_publishers_mutex);
	for(const auto &el : writer_info)
	{
		if(el.first == topic)
		{
			using namespace eprosima::fastrtps;
			using namespace eprosima;
			ParticipantAttributes participant_attr;
			participant_attr.rtps.setName("Participant_subscriber");
			auto custom_transport = std::make_shared<eprosima::fastrtps::rtps::UDPv4TransportDescriptor>();
			custom_transport->interfaceWhiteList.emplace_back("127.0.0.1");
			participant_attr.rtps.useBuiltinTransports = false;
			participant_attr.rtps.userTransports.push_back(custom_transport);

			Participant *participant = Domain::createParticipant(participant_attr);
			// el.second.registerType(particpant);
			// TODO temporarily resgister all the types explicitly like this
			// GaitSignalMsgPubSubType type;
			// Domain::registerType(participant, &type);

			// TODO temporariliy do all of them explicitly
			BlindStateMsgPubSubType      type0;
			ConsoleCommandMsgPubSubType  type1;
			ControlSignalMsgPubSubType   type2;
			DesiredTorquesMsgPubSubType  type3;
			GaitSignalMsgPubSubType      type4;
			// HeaderMsgPubSubType          type5;
			HelloWorldPubSubType         type6;
			ImuMsgPubSubType             type7;
			JointStateMsgPubSubType      type8;
			PoseMsgPubSubType            type9;
			ScrewMsgPubSubType           type10;
			StringMsgPubSubType          type11;
			// TimeMsgPubSubType            type12;
			Vec3MsgPubSubType            type13;
			Domain::registerType(participant, &type0);
			Domain::registerType(participant, &type1);
			Domain::registerType(participant, &type2);
			Domain::registerType(participant, &type3);
			Domain::registerType(participant, &type4);
			// Domain::registerType(participant, &type5);
			Domain::registerType(participant, &type6);
			Domain::registerType(participant, &type7);
			Domain::registerType(participant, &type8);
			Domain::registerType(participant, &type9);
			Domain::registerType(participant, &type10);
			Domain::registerType(participant, &type11);
			// Domain::registerType(participant, &type12);
			Domain::registerType(participant, &type13);

			SubscriberAttributes subscriber_attr;
			subscriber_attr.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
			subscriber_attr.topic.topicName = topic;
			subscriber_attr.topic.topicDataType = el.second.info.typeName();
			DMSG(el.second.info.typeName());

			HzSub subscriber_listener(topic);
			Subscriber *subscriber = Domain::createSubscriber(participant, subscriber_attr, &subscriber_listener);
			auto launch_time = std::chrono::system_clock::now();
			while(true)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
				auto run_time = std::chrono::system_clock::now() - launch_time;
				auto run_time_sec = std::chrono::duration_cast<std::chrono::duration<double>>(run_time);
				// DMSG("Ran for " << run_time_sec.count() << " seconds");
				// DLOG(subscriber_listener.msg_count);
				std::cout << subscriber_listener.msg_count/run_time_sec.count()
					<< " hz" << std::endl;
			}
			auto a = subscriber;
			a += 1;
			a = nullptr;
			return;
		}
	}

	std::cerr << "Topic '" << topic << "' not found" << std::endl;
}

// =============================================================================
// Helper Classes
// =============================================================================
TopicInfo::HzSub::HzSub(const std::string &) :
	msg_count(0)
{
	DMSG("Created sub");
}

void TopicInfo::HzSub::onNewDataMessage(eprosima::fastrtps::Subscriber*)
{
	++msg_count;
}
