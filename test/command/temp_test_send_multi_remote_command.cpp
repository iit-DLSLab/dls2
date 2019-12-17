#include "command/remote_command.hpp"
#include <chrono>
#include <thread>

using namespace dls;

void run()
{
	CommandRegisterMsg msg;
	RemoteCommand rc(msg);
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(int argc, char *argv[])
{
	std::thread t0(run);
	std::thread t1(run);
	std::thread t2(run);
	std::thread t3(run);
	std::thread t4(run);
	std::thread t5(run);
	std::thread t6(run);
	std::thread t7(run);
	std::thread t8(run);
	std::thread t9(run);
	t0.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	return 0;
}
