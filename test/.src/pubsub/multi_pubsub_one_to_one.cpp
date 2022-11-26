

#include "dls2/util/messaging/dds_writer.hpp"
#include "dls2/util/messaging/dds_reader.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <sstream>

using namespace dls;

size_t num_pubsub_pairs           = 50;
size_t number_of_messages_to_send = 100;

int main()
{
	std::vector<dls::DDSReader*> subscribers;
	std::vector<dls::DDSWriter*> publishers;
	std::vector<size_t>      	 count_received_messages;

	count_received_messages.resize(num_pubsub_pairs);

	// each subscriber/publisher pair communicates over this topic with the
	// pair's index appended to it
	std::string topic_base =
		"MULTI_PUBSUB_ONE_TO_ONE_CPP_5f9b9ef5_00af_4757_8562_354f40ed5d6e";

	// create the publisher-subscriber pairs
	std::cout << "========== Building Publisher/Subscriber Pairs =========" << std::endl;
	for(size_t i = 0; i != num_pubsub_pairs; ++i)
	{
		// build the topic name for this pair
		std::stringstream topic_ss;
		topic_ss << topic_base << i;

		// build the publisher for this pair
		auto pub = new DDSWriter(
			"publisher:" + std::to_string(i),
			0,
			topicType("MULTI_PUBSUB_ONE_TO_ONE_CPP:" + std::to_string(i), new StringMsgPubSubType())
		);
		publishers.push_back(pub);

		// build the subscriber for this pair
		auto sub = new DDSReader
		(
			"subscriber:" + std::to_string(i),
			0,
			topicType("MULTI_PUBSUB_ONE_TO_ONE_CPP:" + std::to_string(i), new StringMsgPubSubType()),
			std::function<void(void *)>
			{
				[=, &count_received_messages](void *tuple)
				{
					std::cout << "subscriber " << i << " got a message" << std::endl;
					++count_received_messages[i];			
				}
			}
		);
		subscribers.push_back(sub);
	}

	// run the publishers
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "========== Running the publishers ==========" << std::endl;
	StringMsg msg;
	msg.msg() = "this is the message, yo";
	for(size_t i = 0; i != number_of_messages_to_send; ++i)
	{
		for(const auto &publisher : publishers)
		{
			publisher->sendMessage((void*) &msg);
		}
	}

	// give us a moment to read the responses
	std::this_thread::sleep_for(std::chrono::seconds(3));
	size_t number_of_subscribers_that_missed_communication = 0;
	for(size_t i = 0; i != num_pubsub_pairs; ++i)
	{
		if(count_received_messages[i] != number_of_messages_to_send)
		{
			++number_of_subscribers_that_missed_communication;
		}
	}

	if(number_of_subscribers_that_missed_communication != 0)
	{
		std::cerr << number_of_subscribers_that_missed_communication <<
			" subscribers did not receive all their messages" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
