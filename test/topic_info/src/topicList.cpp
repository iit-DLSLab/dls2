#include "dls2/util/messaging/dds_reader.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>





int main(int argc, char** argv){

    int domain = 3;//dls::domains::signals
	if(argc==2)
	{
		domain = strtol(argv[1], NULL, 10);
	}

    dls::DDSReader sub
	(
		"topic_list",
		domain,
		eprosima::fastrtps::rtps::DiscoveryProtocol_t::SUPER_CLIENT
	);

	sub.printDiscoveredTopics();

    return EXIT_SUCCESS;

}