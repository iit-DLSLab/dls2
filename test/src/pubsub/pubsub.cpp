#include <catch2/catch.hpp>
#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/util/messaging/callback_subscriber.hpp"
#include "dls2/msg/stringmsgPubSubTypes.h"
#include <string>
#include <chrono>

TEST_CASE("Messages can be published and received via topics", "[pubsub]")
{
	std::string topic("this_is_a_pubsub_test_topic_a1212j3jL@#@!jfsxzc");
	std::string send_message("this is the message that needs to be sent");

	SECTION("A message is sent and received")
	{
		std::string received_message("garbage aslkdfjskldafjsklafjlskajfd");
		REQUIRE(received_message != send_message);

		dls::PublisherBase<StringMsgPubSubType> publisher(topic);
		dls::CallbackSubscriber<StringMsgPubSubType> subscriber
		(
			topic,
			[&](StringMsg &msg)
			{
				received_message = msg.msg();
			}
		);

		StringMsg msg;
		msg.msg() = send_message;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		publisher.publish(msg);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		REQUIRE(received_message == send_message);
	}
}
