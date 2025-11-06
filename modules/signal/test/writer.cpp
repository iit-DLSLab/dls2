#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <thread>

#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/signal/writer.hpp"
#include "dls2/topics/topics.hpp"
#include <dls_messages/dds/blind_state.hpp>

int main()
{
	const int domain_id = 3;
	std::shared_ptr<dls::DDSParticipant> dds_participant = std::make_shared<dls::DDSParticipant>("ddsparticipant", domain_id);

	dls::Writer<dls2_interface::msg::BlindState> writer (
		dds_participant,
		dls::topics::low_level_estimation::blind_state);

	writer.msg.joints_position().resize(12);

	int count = 0;
	// check if message has timestamp
	if (writer.hasTimestamp()) {
		std::cout << "Message has a timestamp." << std::endl;
	} else {
		std::cout << "Message does not have a timestamp." << std::endl;
	}
	while(true){
		// Fill the joint positions with random values
		for (int i = 0; i < 12; ++i) {
			writer.msg.joints_position()[i] = i+count;
		}
		// print the joint positions
		std::cout << "Joint positions: ";
		for (const auto& joint_position : writer.msg.joints_position()) {
			std::cout << joint_position << " ";
		}
		std::cout << std::endl;
		count++;
		writer.publish();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}


	return 0;
}
