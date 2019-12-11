#include "controller/controller.hpp"
#include "util/messaging/subscriber_base.hpp"
#include "util/messaging/publisher_base.hpp"
#include "msg/stringmsgPubSubTypes.h"
#include <chrono>
#include <thread>
#include "topics/warn_log_stream.hpp"
#include "util/log/log.hpp"

auto topic = dls::topics::warn_log_stream;

class TestSub : public dls::SubscriberBase<StringMsgPubSubType>
{
public:
	TestSub() :
		dls::SubscriberBase<StringMsgPubSubType>(topic),
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
		std::cout << "got a field message: " << msg.msg() << std::endl;
		std::lock_guard<std::mutex> lock(this->count_mutex);
		++count;
	}

	std::mutex count_mutex;
	size_t count = 0;
};

class TestController : public dls::Controller
{
public:
	TestController() :
		dls::Controller
		(
			std::make_shared<dls::Dog>(),
			"test_controller",
			std::chrono::duration<double, std::ratio<1,1>>(1),
			dls::ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
		),
		pub1(topic),
		pub2(topic),
		pub3(topic),
		pub4(topic),
		pub5(topic),
		pub6(topic),
		pub7(topic),
		pub8(topic),
		pub9(topic),
		scout1("test_controller"),
		scout2("test_controller"),
		scout3("test_controller"),
		scout4("test_controller"),
		scout5("test_controller"),
		scout6("test_controller"),
		scout7("test_controller"),
		scout8("test_controller"),
		scout9("test_controller")
	{ }

	void run(const std::chrono::system_clock::time_point &) override
	{
		std::cout << "run" << std::endl;
		StringMsg msg;
		msg.msg("hello world");
		pub1.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(1));
		pub2.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		pub3.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		pub4.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		pub5.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		pub6.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		pub7.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		pub8.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		pub9.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));

		std::this_thread::sleep_for(std::chrono::seconds(1));

		scout1 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(1));
		scout2 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		scout3 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		scout4 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		scout5 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		scout6 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		scout7 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		scout8 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		scout9 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
	}

	Status eStop() override { return getStatus(); }

private:
	dls::PublisherBase<StringMsgPubSubType> pub1;
	dls::PublisherBase<StringMsgPubSubType> pub2;
	dls::PublisherBase<StringMsgPubSubType> pub3;
	dls::PublisherBase<StringMsgPubSubType> pub4;
	dls::PublisherBase<StringMsgPubSubType> pub5;
	dls::PublisherBase<StringMsgPubSubType> pub6;
	dls::PublisherBase<StringMsgPubSubType> pub7;
	dls::PublisherBase<StringMsgPubSubType> pub8;
	dls::PublisherBase<StringMsgPubSubType> pub9;
	dls::logging::coutstream scout1;
	dls::logging::coutstream scout2;
	dls::logging::coutstream scout3;
	dls::logging::coutstream scout4;
	dls::logging::coutstream scout5;
	dls::logging::coutstream scout6;
	dls::logging::coutstream scout7;
	dls::logging::coutstream scout8;
	dls::logging::coutstream scout9;
};

int main(int argc, char *argv[])
{
	size_t loop_count = 10;
	TestSub test_sub;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	TestController tc;
	for(size_t i = 0; i != loop_count; ++i)
	{
		tc.run(std::chrono::system_clock::now());
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	if(test_sub.getCount() != loop_count*9*2)
	{
		std::cout << "Got: " << test_sub.getCount() << " expected: " << loop_count*9*2 << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
