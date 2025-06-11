#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/string_msgPubSubTypes.hpp"
#include "dls_messages/dds/string_msg.hpp"

#include <thread>
#include <any>

int main()
{
    dls::DDSParticipant participant("participant", 0);
    auto writer = participant.addWriter("dds_writer", dls::topicType("rt/chatter", new std_msgs::msg::StringPubSubType()));
    std_msgs::msg::String msg;

    int count = 0;
    while(true)
    {
        std::string current_msg = "Fastdds writer is sending you a message with index " + std::to_string(count);
        msg.data() = current_msg;
        writer->write(&msg);
        std::cout << current_msg << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        count++;
    }
    return 0;
}