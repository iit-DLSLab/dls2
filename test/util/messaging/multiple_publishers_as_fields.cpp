#include "util/messaging/publisher_base.hpp"
#include "util/messaging/subscriber_base.hpp"
#include "msg/boolPubSubTypes.h"
#include <thread>
#include <chrono>

constexpr auto topic = "sadlfhsahfdoas2313kahfsf0sda7fa9s6432ui4g2qj";
using namespace dls;

class TestPubs
{
public:
	TestPubs():
		pub1(topic),
		pub2(topic),
		pub3(topic),
		pub4(topic),
		pub5(topic),
		pub6(topic),
		pub7(topic),
		pub8(topic),
		pub9(topic)//,
		// pub10(topic)
	{
		BoolMsg b;
		b.val(true);

		pub1.publish(b);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		pub2.publish(b);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		pub3.publish(b);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		pub4.publish(b);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		pub5.publish(b);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		pub6.publish(b);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		pub7.publish(b);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		pub8.publish(b);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		pub9.publish(b);
		// std::this_thread::sleep_for(std::chrono::seconds(1));
		// pub10.publish(b);
	}

private:
	PublisherBase<BoolMsgPubSubType> pub1;
	PublisherBase<BoolMsgPubSubType> pub2;
	PublisherBase<BoolMsgPubSubType> pub3;
	PublisherBase<BoolMsgPubSubType> pub4;
	PublisherBase<BoolMsgPubSubType> pub5;
	PublisherBase<BoolMsgPubSubType> pub6;
	PublisherBase<BoolMsgPubSubType> pub7;
	PublisherBase<BoolMsgPubSubType> pub8;
	PublisherBase<BoolMsgPubSubType> pub9;
	// PublisherBase<BoolMsgPubSubType> pub10;
};

class TestSub : public SubscriberBase<BoolMsgPubSubType>
{
public:
	TestSub() :
		SubscriberBase<BoolMsgPubSubType>(topic),
		count(0)
	{ }

	size_t getCount()
	{
		std::lock_guard<std::mutex> lock(this->count_mutex);
		return count;
	}
private:
	void onNewDataMessage(eprosima::fastrtps::Subscriber*) override
	{
		std::cout << "got a message" << std::endl;
		std::lock_guard<std::mutex> lock(this->count_mutex);
		++count;
	}

	std::mutex count_mutex;
	size_t count = 0;
};

int main(int argc, char *argv[])
{
	TestSub sub;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	TestPubs t;
	std::this_thread::sleep_for(std::chrono::seconds(3));

	if(sub.getCount() != 9)
	{
		std::cout << "count: " << sub.getCount() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
