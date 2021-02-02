#include <catch2/catch.hpp>
#include "dls2/msg/stringmsgPubSubTypes.h"
#include "dls2/util/messaging/callback_subscriber.hpp"
#include "dls2/util/messaging/publisher_base.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

size_t number_of_subscribers      = 2;
size_t number_of_messages_to_send = 10;

TEST_CASE("A single publisher can publish to multiple subscribers", "[pubsub]")
{
	std::cout << "creating objects" << std::endl;
	std::string topic("topic_pub_multisub");
	std::string send_message("this is the message that needs to be sent");

	dls::PublisherBase<StringMsgPubSubType> publisher(topic);
	std::vector
	<
		dls::CallbackSubscriber<StringMsgPubSubType>
	> subscribers;

	std::vector<size_t> counts;
	counts.resize(number_of_subscribers + 10);

	dls::CallbackSubscriber<StringMsgPubSubType> sub
	(
		topic,
		[=, &counts](StringMsg &msg)
		{
			std::cout << "Sub " << 0 << " got a message" << std::endl;
			REQUIRE(msg.msg() == send_message);
			++counts[0];
		}
	);
	dls::CallbackSubscriber<StringMsgPubSubType> sub1
	(
		topic,
		[=, &counts](StringMsg &msg)
		{
			std::cout << "Sub " << 1 << " got a message" << std::endl;
			REQUIRE(msg.msg() == send_message);
			++counts[1];
		}
	);

	SECTION("A message is sent and received by multiple subscribers")
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "Starting to publish..." << std::endl;

		for(size_t i = 0; i != number_of_messages_to_send; ++i)
		{
			StringMsg msg;
			msg.msg() = send_message;
			std::cout << "publish " << i << "..." << std::endl;
			publisher.publish(msg);
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "Done publishing, checking results" << std::endl;

		for(size_t i = 0; i != number_of_subscribers; ++i)
		{
			REQUIRE(counts[i] == number_of_messages_to_send);
		}
	}
}
