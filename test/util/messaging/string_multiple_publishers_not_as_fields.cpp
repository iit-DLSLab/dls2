#include "util/messaging/publisher_base.hpp"
#include "util/messaging/subscriber_base.hpp"
#include "msg/stringmsgPubSubTypes.h"
#include "topics/warn_log_stream.hpp"
#include <thread>
#include <chrono>
#include "util/log/log.hpp"

constexpr auto topic = dls::topics::warn_log_stream;
using namespace dls;

class TestSub : public SubscriberBase<StringMsgPubSubType>
{
public:
	TestSub() :
		SubscriberBase<StringMsgPubSubType>(topic),
		count(0)
	{ }

	size_t getCount()
	{
		std::lock_guard<std::mutex> lock(this->count_mutex);
		return count;
	}
private:
	void onNewDataMessage(eprosima::fastrtps::Subscriber *s) override
	{
		StringMsg msg;
		s->takeNextData(&msg, nullptr);
		std::cout << "got a non-field message: " << msg.msg() << std::endl;
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

	StringMsg b;
	b.msg("Hello world");

	PublisherBase<StringMsgPubSubType> pub1(topic);
	PublisherBase<StringMsgPubSubType> pub2(topic);
	PublisherBase<StringMsgPubSubType> pub3(topic);
	PublisherBase<StringMsgPubSubType> pub4(topic);
	PublisherBase<StringMsgPubSubType> pub5(topic);
	PublisherBase<StringMsgPubSubType> pub6(topic);
	PublisherBase<StringMsgPubSubType> pub7(topic);
	PublisherBase<StringMsgPubSubType> pub8(topic);
	PublisherBase<StringMsgPubSubType> pub9(topic);
	logging::coutstream stream1("string_multiple_publishers_not_as_fields");
	logging::coutstream stream2("string_multiple_publishers_not_as_fields");
	logging::coutstream stream3("string_multiple_publishers_not_as_fields");
	logging::coutstream stream4("string_multiple_publishers_not_as_fields");
	logging::coutstream stream5("string_multiple_publishers_not_as_fields");
	logging::coutstream stream6("string_multiple_publishers_not_as_fields");
	logging::coutstream stream7("string_multiple_publishers_not_as_fields");
	logging::coutstream stream8("string_multiple_publishers_not_as_fields");
	logging::coutstream stream9("string_multiple_publishers_not_as_fields");
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

	stream1 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
	stream2 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
	stream3 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
	stream4 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
	stream5 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
	stream6 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
	stream7 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
	stream8 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
	stream9 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
	// pub10.publish(b);std::this_thread::sleep_for(std::chrono::seconds(1));

	std::this_thread::sleep_for(std::chrono::seconds(3));

	if(sub.getCount() != 9*2)
	{
		std::cout << "count: " << sub.getCount() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
