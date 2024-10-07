
#include "dls2/util/messaging/dds_writer.hpp"
#include "dls2/util/messaging/dds_reader.hpp"

#include <catch2/catch.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>

size_t number_of_subscribers      = 20;
size_t number_of_messages_to_send = 10;

TEST_CASE("A single publisher can publish to multiple subscribers", "[pubsub]")
{
	std::cout << "creating objects" << std::endl;
	dls::topicType topic("topic_pub_multisub", new StringMsgPubSubType());
	std::string send_message("this is the message that needs to be sent");

	dls::DDSParticipant server("test_server", 0, eprosima::fastrtps::rtps::DiscoveryProtocol_t::SERVER);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	dls::DDSWriter publisher("test_publisher", 0, topic);

	std::vector<std::shared_ptr<dls::DDSReader>> subscribers;

	std::vector<size_t> counts;
	counts.resize(number_of_subscribers + 10);

	for(size_t i=0; i < number_of_subscribers; i++)
	{
		std::shared_ptr<dls::DDSReader> sub = std::make_shared<dls::DDSReader>
		(
			"reader" + std::to_string(i+1),
			0,
			topic,
			std::function<void(void *)>
			{
				[=, &counts](void *tuple)
				{
					StringMsg msg = *((StringMsg *) tuple);

					std::cout << "Sub " << std::to_string(i) << " got a message" << std::endl;
					REQUIRE(msg.msg() == send_message);
					++counts[i];			
				}
			}
		);

		subscribers.push_back(sub);
	}

	SECTION("A message is sent and received by multiple subscribers")
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "Starting to publish..." << std::endl;

		for(size_t i = 0; i != number_of_messages_to_send; ++i)
		{
			StringMsg msg;
			msg.msg() = send_message;
			std::cout << "publish " << i << "..." << std::endl;
			publisher.sendMessage((void*) &msg);
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "Done publishing, checking results" << std::endl;

		for(size_t i = 0; i != number_of_subscribers; ++i)
		{
			REQUIRE(counts[i] == number_of_messages_to_send);
		}
	}
}
