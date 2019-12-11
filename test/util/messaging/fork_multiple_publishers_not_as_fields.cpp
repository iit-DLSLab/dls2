#include "util/messaging/publisher_base.hpp"
#include "util/messaging/subscriber_base.hpp"
#include "msg/boolPubSubTypes.h"
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

constexpr auto topic = "sadlfhsahfdoas2313kahfsf0sda7fa9s6432ui4g2qj";
using namespace dls;

class TestSub : public SubscriberBase<BoolMsgPubSubType>
{
public:
	TestSub() :
		SubscriberBase<BoolMsgPubSubType>(topic)
	{ }

	size_t getCount()
	{
		std::lock_guard<std::mutex> lock(this->count_mutex);
		return count;
	}
private:
	void onNewDataMessage(eprosima::fastrtps::Subscriber*) override
	{
		std::cout << "got a fork non-field message: " << count << std::endl;
		std::lock_guard<std::mutex> lock(this->count_mutex);
		++count;
		std::cout << "post got a fork non-field message: " << count << std::endl;
	}

	static std::mutex count_mutex;
	static size_t count;
};

size_t TestSub::count = 0;
std::mutex TestSub::count_mutex;

int main(int argc, char *argv[])
{
	auto pid = fork();

	if(pid == 0)
	{
		std::this_thread::sleep_for(std::chrono::seconds(3));

		BoolMsg b;
		b.val(true);

		PublisherBase<BoolMsgPubSubType> pub1(topic);
		PublisherBase<BoolMsgPubSubType> pub2(topic);
		PublisherBase<BoolMsgPubSubType> pub3(topic);
		PublisherBase<BoolMsgPubSubType> pub4(topic);
		PublisherBase<BoolMsgPubSubType> pub5(topic);
		PublisherBase<BoolMsgPubSubType> pub6(topic);
		PublisherBase<BoolMsgPubSubType> pub7(topic);
		PublisherBase<BoolMsgPubSubType> pub8(topic);
		PublisherBase<BoolMsgPubSubType> pub9(topic);
		// PublisherBase<BoolMsgPubSubType> pub10(topic);

		pub1.publish(b); std::this_thread::sleep_for(std::chrono::seconds(1));
		pub2.publish(b); std::this_thread::sleep_for(std::chrono::seconds(1));
		pub3.publish(b); std::this_thread::sleep_for(std::chrono::seconds(1));
		pub4.publish(b); std::this_thread::sleep_for(std::chrono::seconds(1));
		pub5.publish(b); std::this_thread::sleep_for(std::chrono::seconds(1));
		pub6.publish(b); std::this_thread::sleep_for(std::chrono::seconds(1));
		pub7.publish(b); std::this_thread::sleep_for(std::chrono::seconds(1));
		pub8.publish(b); std::this_thread::sleep_for(std::chrono::seconds(1));
		pub9.publish(b); std::this_thread::sleep_for(std::chrono::seconds(1));
		// pub10.publish(b);std::this_thread::sleep_for(std::chrono::seconds(1));

		std::this_thread::sleep_for(std::chrono::seconds(3));

		_exit(EXIT_SUCCESS);
	}

	TestSub sub;

	int status;
	wait(&status);

	if(sub.getCount() != 9)
	{
		std::cout << "count: " << sub.getCount() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
