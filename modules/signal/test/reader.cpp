#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <thread>

#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/signal/reader.hpp"
#include "dls2/topics/topics.hpp"
#include "dls_messages/dds/blind_state.hpp"

int main()
{
	const int domain_id = 3;
	dls::DDSParticipant server("server", domain_id, eprosima::fastdds::rtps::DiscoveryProtocol::SERVER);

	std::shared_ptr<dls::DDSParticipant> dds_participant = std::make_shared<dls::DDSParticipant>("ddsparticipant", domain_id);

	dls::Reader<BlindState> reader (
		dds_participant,
		dls::topics::low_level_estimation::blind_state);

	while(true){
		if (!reader.is_receiving_data()) {
			std::cout << "No data received yet." << std::endl;
		}
		else{
			reader.read();
			std::vector<double> q = reader.msg.joints_position();
			// print the joint positions
			std::cout << "Joint positions: ";
			for (const auto& joint_position : q) {
				std::cout << joint_position << " ";
			}
			std::cout << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}


	return 0;
}
