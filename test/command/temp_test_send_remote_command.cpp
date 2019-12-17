#include "command/remote_command.hpp"
#include <chrono>
#include <thread>

using namespace dls;
int main(int argc, char *argv[])
{
	CommandRegisterMsg msg;
	RemoteCommand rc(msg);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}
