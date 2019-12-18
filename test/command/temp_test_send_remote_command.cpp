#include "command/remote_command.hpp"
#include <chrono>
#include <thread>

using namespace dls;
int main(int argc, char *argv[])
{
	CommandRegisterMsg msg;
	msg.arg_types().push_back
	(
		static_cast
		<
			std::remove_reference<decltype(msg.arg_types())>::type::value_type
		>
		(
			CommandBase::ArgumentType::UINT32
		)
	);
	msg.arg_types().push_back
	(
		static_cast
		<
			std::remove_reference<decltype(msg.arg_types())>::type::value_type
		>
		(
			CommandBase::ArgumentType::STD_STRING
		)
	);

	RemoteCommand rc(msg);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}
