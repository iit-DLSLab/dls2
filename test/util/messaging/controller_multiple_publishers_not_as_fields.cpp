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
		// s->takeNextData(&msg, nullptr);
		// std::cout << "got a non-field message: " << msg.msg() << std::endl;
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
		)
	{ }

	void run(const std::chrono::system_clock::time_point &) override
	{
		std::cout << "construct publishers" << std::endl;
		dls::PublisherBase<StringMsgPubSubType> pub1(topic);
		std::cout << 1 << std::endl;
		dls::PublisherBase<StringMsgPubSubType> pub2(topic);
		std::cout << 2 << std::endl;
		dls::PublisherBase<StringMsgPubSubType> pub3(topic);
		std::cout << 3 << std::endl;
		dls::PublisherBase<StringMsgPubSubType> pub4(topic);
		std::cout << 4 << std::endl;
		dls::PublisherBase<StringMsgPubSubType> pub5(topic);
		std::cout << 5 << std::endl;
		dls::PublisherBase<StringMsgPubSubType> pub6(topic);
		std::cout << 6 << std::endl;
		dls::PublisherBase<StringMsgPubSubType> pub7(topic);
		std::cout << 7 << std::endl;
		dls::PublisherBase<StringMsgPubSubType> pub8(topic);
		std::cout << 8 << std::endl;
		dls::PublisherBase<StringMsgPubSubType> pub9(topic);
		std::cout << 9 << std::endl;
		dls::logging::coutstream scout1("test_controller");
		std::cout << 10 << std::endl;
		dls::logging::coutstream scout2("test_controller");
		std::cout << 11 << std::endl;
		dls::logging::coutstream scout3("test_controller");
		std::cout << 12 << std::endl;
		dls::logging::coutstream scout4("test_controller");
		std::cout << 13 << std::endl;
		dls::logging::coutstream scout5("test_controller");
		std::cout << 14 << std::endl;
		dls::logging::coutstream scout6("test_controller");
		std::cout << 15 << std::endl;
		dls::logging::coutstream scout7("test_controller");
		std::cout << 16 << std::endl;
		dls::logging::coutstream scout8("test_controller");
		std::cout << 17 << std::endl;
		dls::logging::coutstream scout9("test_controller");
		std::cout << 18 << std::endl;
		std::cout << "done construct publishers" << std::endl;

		// std::cout << "run" << std::endl;
		StringMsg msg;
		msg.msg("hello world");
		std::cout << "run publish on normal publishers" << std::endl;
		pub1.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 1 << std::endl;
		pub2.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 2 << std::endl;
		pub3.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 3 << std::endl;
		pub4.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 4 << std::endl;
		pub5.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 5 << std::endl;
		pub6.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 6 << std::endl;
		pub7.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 7 << std::endl;
		pub8.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 8 << std::endl;
		pub9.publish(msg); std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 9 << std::endl;
		std::cout << "done run publish on normal publishers" << std::endl;

		// std::this_thread::sleep_for(std::chrono::seconds(1));

		std::cout << "run publish on stream publishers" << std::endl;
		scout1 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 1 << std::endl;
		scout2 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 2 << std::endl;
		scout3 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 3 << std::endl;
		scout4 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 4 << std::endl;
		scout5 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 5 << std::endl;
		scout6 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 6 << std::endl;
		scout7 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 7 << std::endl;
		scout8 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 8 << std::endl;
		scout9 << "Hello world stream" << std::flush; std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << 9 << std::endl;
		std::cout << "done run publish on stream publishers" << std::endl;

		// dls::ControlSignal s;
		// s.torques.resize(12);

		// publishSignal(s);
	}

	Status eStop() override { return getStatus(); }

private:
};

int main(int argc, char *argv[])
{
	size_t loop_count = 3;
	TestSub test_sub;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	TestController tc;
	for(size_t i = 0; i != loop_count; ++i)
	{
		tc.run(std::chrono::system_clock::now());
		std::cout << "finsihed run" << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1,1000>>(1));
		// std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "========================================" << std::endl;
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << "Got: " << test_sub.getCount() << " expected: " << loop_count*9*2 << std::endl;
	if(test_sub.getCount() != loop_count*9*2)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
