#include "topics/command_register.hpp"
#include "msg/command_registerPubSubTypes.h"
#include "util/messaging/subscriber_base.hpp"

#include "msg/stringmsgPubSubTypes.h"

#include <iostream>
#include <chrono>

using namespace dls;

struct listener : public SubscriberBase<CommandRegisterMsgPubSubType>
// struct listener : public SubscriberBase<StringMsgPubSubType>
{
	listener() :
		SubscriberBase<CommandRegisterMsgPubSubType>(topics::command_register)
		// SubscriberBase<StringMsgPubSubType>(topics::command_register)
	{ }

	void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override
	{
		std::cout << "hit message" << std::endl;
	}
};

int main(int argc, char *argv[])
{

	listener l;
	for(int i = 0; i != 20; ++i)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}
