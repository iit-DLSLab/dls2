#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include "dls2/util/messaging/dds_participant.hpp"

#include <unistd.h>

#include "dls2/topics/topics.hpp"
#include "dls2/command/command_base.hpp"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;

int main(
        int argc,
        char** argv)
{
    std::cout << "Starting DLS2 CLIENT." << std::endl;

	dls::DDSParticipant participant("DLS2 CLIENT", dls::domains::layers);

	const std::string writer_name("dls2 writer");
	participant.addWriter(writer_name, dls::topics::command_call);

	// const std::string reader_name("dls2 reader");
	// participant.addReader(reader_name, dls::topics::command_call, nullptr);

    int num_prints = 0;
    CommandCallMsg command;
	while(true)
    {
		// participant.sendMessage(writer_name, static_cast<void*>(&command));

        auto participant_names = participant.getParticipants();
        std::cout << "DISCOVERED PARTICIPANT NAMES:" << std::endl;
        for(auto name : participant_names)
        {
            std::cout << name << std::endl;
        }
        num_prints++;
        sleep(0.7);
    }
    return 0;
}