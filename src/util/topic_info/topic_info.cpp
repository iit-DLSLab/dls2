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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
// TODO this is a tool that needs to be made into a separate project and made
// more sophisticated
#include "util/topic_info/topic_info.hpp"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>

using namespace dls;

TopicInfo::TopicInfo() :
	topics_publishers_mutex(),
	writer_info(),
	topics_subscribers_mutex(),
	reader_info()
{

	eprosima::fastrtps::ParticipantAttributes participant_attr;
	participant_attr.rtps.setName("Participant_publisher");
	auto custom_transport = std::make_shared<eprosima::fastrtps::rtps::UDPv4TransportDescriptor>();
	custom_transport->interfaceWhiteList.emplace_back("127.0.0.1");
	participant_attr.rtps.useBuiltinTransports = false;
	participant_attr.rtps.userTransports.push_back(custom_transport);

	eprosima::fastrtps::Domain::createParticipant(participant_attr, this);
}

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
