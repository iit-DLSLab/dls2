#include "command/command.hpp"

#include "msg/stringmsgPubSubTypes.h"
using namespace dls;


int foo(int)
{
	return 1;
}
int main(int argc, char *argv[])
{
	// PublisherBase<StringMsgPubSubType> pub(topics::command_register);
	// StringMsg msg;
	// pub.publish(msg);
	Command<int, int> command("test", "dummy", "help", foo);
	command.requestRegistration();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}
