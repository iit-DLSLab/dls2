#include <catch2/catch.hpp>
#include "dls2/util/messaging/dds_writer.hpp"
#include "dls2/util/messaging/dds_reader.hpp"

#include <string>
#include <chrono>

TEST_CASE("Messages can be published and received via topics", "[pubsub]")
{
	dls::topicType topic("this_is_a_pubsub_test_topic_a1212j3jL@#@!jfsxzc", new StringMsgPubSubType());
	std::string send_message("this is the message that needs to be sent");

	SECTION("A message is sent and received")
	{
		std::string received_message("garbage aslkdfjskldafjsklafjlskajfd");
		REQUIRE(received_message != send_message);

		dls::DDSWriter publisher("test_writer", dls::domains::develop, topic);
		dls::DDSReader subscriber("test_reader", dls::domains::develop, topic,
			std::function<void(void *)>
			{
				[&](void *tuple)
				{
					received_message = ((StringMsg*) tuple)->msg();
				}
			}
		);

		StringMsg msg;
		msg.msg() = send_message;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		publisher.sendMessage(&msg);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		REQUIRE(received_message == send_message);
	}
}
