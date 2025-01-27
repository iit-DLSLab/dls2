#include "dls2/util/messaging/dds_writer.hpp"
#include "dls2/util/messaging/dds_reader.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <thread>

dls::topicType topic("this_is_a_long_dummy_test_topic", new StringMsgPubSubType());
auto target_message = "this is the required message that has to be delivered";
constexpr size_t COUNT_OF_SUBSCRIBERS             = 10;
constexpr size_t COUNT_OF_PUBLISHERS              = 10;
constexpr size_t NUMBER_OF_MESSAGES_PER_PUBLISHER = 5;
constexpr size_t TOLERANCE_OF_MISSED_MESSAGES     = 5;
constexpr auto   MINIMUM_EXPECTED_MESSAGES        = NUMBER_OF_MESSAGES_PER_PUBLISHER * COUNT_OF_PUBLISHERS - TOLERANCE_OF_MISSED_MESSAGES;

constexpr size_t EXPECTED_NUMBER_OF_MESSAGES_RECEIVED =
    COUNT_OF_PUBLISHERS * NUMBER_OF_MESSAGES_PER_PUBLISHER;

struct SubscriberTestStruct
{
	SubscriberTestStruct();
	size_t count_of_received_messages{0};
	dls::DDSReader subscriber;
};

SubscriberTestStruct::SubscriberTestStruct():
    subscriber(
		"subscriber",
		0,
		topic,
		std::function<void(void *)>
		{
			[&](void* tuple)
			{
				StringMsg msg = *((StringMsg*) tuple);
				if(msg.msg() == target_message)
				{
					++this->count_of_received_messages;
				}
				else
				{
					std::cerr << "Error: message with incorrect payload received" << std::endl;
				}
			}
		}
    )
{ }

int main(int /*argc*/, char ** /*argv*/)
{
	// ================= Build the publishers and subscribers ==================
	std::cout << "Constructing " << COUNT_OF_SUBSCRIBERS << " subscribers and "
	          << COUNT_OF_PUBLISHERS << " publishers" << std::endl;

	dls::DDSParticipant server("test_server", 0, eprosima::fastdds::rtps::DiscoveryProtocol::SERVER);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::vector<std::unique_ptr<dls::DDSWriter>> 		publishers;
	std::vector<std::unique_ptr<SubscriberTestStruct>>  subscribers;

	for(size_t i = 0; i != COUNT_OF_PUBLISHERS; ++i)
	{
		publishers.push_back(
			std::make_unique<dls::DDSWriter>("publisher", 0, topic)
		);
	}

	for(size_t i = 0; i != COUNT_OF_SUBSCRIBERS; ++i)
	{
		subscribers.push_back
		(
			std::make_unique<SubscriberTestStruct>()
		);
	}
	std::this_thread::sleep_for(std::chrono::seconds(2));
	// ============ Ensure that the subscribers are all zeroed out =============
	std::cout << "ensuring that all the subscibers are zeroed correctly... " << std::flush;
	for(const auto &subscriber: subscribers)
	{
		if(subscriber->count_of_received_messages != 0)
		{
			std::cerr << "Error: subscribers are not built with zeroed out "
			             "message counts"
			          << std::endl;
			return EXIT_FAILURE;
		}
	}
	std::cout << "done" << std::endl;

	// =========================== Send the messages ===========================
	std::cout << "Sending out " << NUMBER_OF_MESSAGES_PER_PUBLISHER
	          << " messages for each of the " << COUNT_OF_PUBLISHERS
	          << " publishers" << std::endl;
	StringMsg msg;
	msg.msg() = target_message;
	for(const auto &publisher: publishers)
	{
		for(size_t i = 0; i != NUMBER_OF_MESSAGES_PER_PUBLISHER; ++i)
		{
			publisher->sendMessage((void*) &msg);
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		std::cout << "next publisher" << std::endl;
	}

	// ============== Check whether all the communication worked ===============
	// sleep to give all the communications time to happen. This is just for
	// sanity.
	std::cout << "sleeping for a few seconds to allow fastdds time to finish "
	             "the high level of communication traffic"
	          << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::cout << "Done waiting, starting checks..." << std::endl;


	bool test_was_successful = true;
	for(size_t i = 0; i != COUNT_OF_SUBSCRIBERS; ++i)
	{
		if(subscribers[i]->count_of_received_messages <
		   MINIMUM_EXPECTED_MESSAGES)
		{
			test_was_successful = false;
			std::cerr << "Error, subscriber " << i << " received "
					  << subscribers[i]->count_of_received_messages
					  << ", expected " << EXPECTED_NUMBER_OF_MESSAGES_RECEIVED
					  << std::endl;
		}
	}

	if(!test_was_successful)
	{
		return EXIT_FAILURE;
	}

	std::cout << "Test executed successfully" << std::endl;
	return EXIT_SUCCESS;
}
