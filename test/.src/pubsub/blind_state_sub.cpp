

#include "dls2/util/messaging/dds_reader.hpp"

#include <iostream>

int main()
{
	dls::DDSReader sub
	(
		"dds_subscriber",
		0,
		dls::topics::low_level_estimation::blind_state,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				std::cout << "Got a blind state" << std::endl;			
			}
		}
	);

	int i;
	std::cin >>i;

	return EXIT_SUCCESS;
}
