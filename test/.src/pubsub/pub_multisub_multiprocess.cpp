#include "dls2/util/messaging/dds_writer.hpp"
#include "dls2/util/messaging/dds_reader.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <atomic>
#include <catch2/catch.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

// ================================== Globals ==================================
dls::topicType topic("topic_pub_multisub", new StringMsgPubSubType());
std::string send_message("this is the message that needs to be sent");
const int number_of_messages_to_send = 10;

// ================================ Prototypes =================================
int run_publisher();
int run_subscriber(int sub_id);

// =================================== Main ====================================
int main()
{
	auto publisher_pid = fork();

	if(publisher_pid == 0)
	{
		run_publisher();
	}

	auto subscriber1_pid = fork();
	if(subscriber1_pid == 0)
	{
		run_subscriber(1);
	}

	auto subscriber2_pid = fork();
	if(subscriber2_pid == 0)
	{
		run_subscriber(2);
	}

	for(int i = 0; i != 3; ++i)
	{
		int child_exit_status;
		auto pid = wait(&child_exit_status);

		if(pid == -1)
		{
			std::cerr << "ERROR on wait" << std::endl;
			return EXIT_FAILURE;
		}

		// if the child did not exit normally
		if(!WIFEXITED(child_exit_status))
		{
			std::cerr << "child did not exit normally" << std::endl;
			return EXIT_FAILURE;
		}

		if(WEXITSTATUS(child_exit_status) != EXIT_SUCCESS)
		{
			std::cerr << "child failed" << std::endl;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

// ============================== Child Processes ==============================
// --------------------------------- publisher ---------------------------------
int run_publisher()
{
	dls::DDSWriter publisher(
		"publicher_example",
		dls::domains::develop, 
		topic);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	for(size_t i = 0; i != number_of_messages_to_send; ++i)
	{
		StringMsg msg;
		msg.msg() = send_message;
		std::cout << "publish " << i << "..." << std::endl;
		publisher.sendMessage((void*) &msg);
	}

	exit(EXIT_SUCCESS);
}

// -------------------------------- subscriber ---------------------------------
int run_subscriber(int sub_id)
{
	std::atomic_int sent_count(0);

	dls::DDSReader sub
	(
		"reader_example",
		dls::domains::develop,
		topic,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				StringMsg msg = *((StringMsg*) tuple);

				std::cout << "Sub " << sub_id << " got a message" << std::endl;
				++sent_count;			
			}
		}
	);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	if(sent_count != number_of_messages_to_send)
	{
		exit(EXIT_FAILURE);
	}
	else
	{
		exit(EXIT_SUCCESS);
	}
}
