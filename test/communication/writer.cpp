#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/BaseReferencesPubSubTypes.hpp"

#include <thread>
#include <any>

int main()
{
    dls::DDSParticipant participant("participant", 0);
    auto writer = participant.addWriter("dds_writer", dls::topicType("Sample", new dls2_interface::msg::BaseReferencesPubSubType()));
    dls2_interface::msg::BaseReferences msg;
    msg.sequence_id() = 0;
    msg.timestamp() = 1;
    msg.base_reference()[0] = 0;
    msg.base_reference()[1] = 1;
    msg.base_reference()[2] = 2;
    int count = 0;
    while(true)
    {
        msg.sequence_id() = count;
        msg.timestamp()  = count +1;
        msg.base_reference()[0] = count + 0;
        msg.base_reference()[1] = count + 1;
        msg.base_reference()[2] = count + 2;
        count++;
        
        writer->write(&msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}