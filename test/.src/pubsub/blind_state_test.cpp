#include "dls2/util/messaging/publisher.hpp"
#include "dls2/msg/blind_statePubSubTypes.h"
#include "dls2/msg/timePubSubTypes.h"
#include "dls2/topics/topics.hpp"
#include <chrono>

int main()
{
	dls::PublisherBase<BlindStateMsgPubSubType> blind_state_pub
	(
		"low_level_estimation/blind_state"
	);
	BlindStateMsg blind_state_msg;

	size_t i = 0;
	while(true)
	{
		std::cout << "publishing blind state message " << ++i << std::endl;
		blind_state_pub.publish(blind_state_msg);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	return EXIT_SUCCESS;
}
