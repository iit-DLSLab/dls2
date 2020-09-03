#include "dls2/topics/low_level_estimation/blind_state.hpp"
#include "dls2/util/messaging/callback_subscriber.hpp"
#include "dls2/fastrtps_wrappers/blind_state.hpp"
#include "dls2/msg/blind_statePubSubTypes.h"

#include <iostream>

int main()
{
	dls::CallbackSubscriber<BlindStateMsgPubSubType>
	(
		dls::topics::low_level_estimation::blind_state,
		[&](BlindStateMsg)
		{
			std::cout << "Got a blind state" << std::endl;
		}
	);

	int i;
	std::cin >>i;

	return EXIT_SUCCESS;
}
