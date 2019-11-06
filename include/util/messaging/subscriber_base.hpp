#ifndef SUBSCRIBER_BASE_HPP_XPACOJJI
#define SUBSCRIBER_BASE_HPP_XPACOJJI

#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/subscriber/SampleInfo.h>

template <class PubSub_t>
class SubscriberBase : public eprosima::fastrtps::SubscriberListener
{
public:
	SubscriberBase(const std::string &topic);
	virtual ~SubscriberBase() = default;

private:
	std::shared_ptr<eprosima::fastrtps::Participant> pParticipant;
	std::shared_ptr<eprosima::fastrtps::Subscriber> pSubscriber;

	static PubSub_t rtps_type;
};

#include "util/messaging/subscriber_base.tpp"

#endif /* end of include guard: SUBSCRIBER_BASE_HPP_XPACOJJI */
