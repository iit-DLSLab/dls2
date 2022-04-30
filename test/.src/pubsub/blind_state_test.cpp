

#include "dls2/util/messaging/dds_writer.hpp"
#include <chrono>

int main()
{
	dls::DDSWriter blind_state_pub(
		"dds_publisher",
		dls::domains::develop,
		dls::topics::low_level_estimation::blind_state
	);

	BlindStateMsg blind_state_msg;

	size_t i = 0;
	while(true)
	{
		std::cout << "publishing blind state message " << ++i << std::endl;
		blind_state_pub.sendMessage((void*) &blind_state_msg);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	return EXIT_SUCCESS;
}
