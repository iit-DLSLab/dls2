#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/string_msgPubSubTypes.hpp"

#include <thread>
#include <any>

// 3 main steps to connect FastDDS to ROS2
// 1. generate .idl from .msg file:
//      rosidl translate --to idl -o . package_name message_name.msg
// 2. generate fastdds message using the -typeros2 option (-cs is used to allow case-sentitive names): 
//      fastddsgen -replace -cs -typeros2 message_name.idl
// 3. create a fastdd reader/writer connected to a topic with name rt/ros2_topic_name
// NB: a reader/writer could be configured to interface with ROS2. This means that the rt/ is automatically added before the topic name

int main()
{
    dls::DDSParticipant participant("participant", 0);
    participant.addReader("dds_reader", dls::topicType("rt/chatter", new std_msgs::msg::StringPubSubType()),std::function<void(void *)>
    {
        [&](void *msg)
        {
            std_msgs::msg::String string_msg = *(static_cast<std_msgs::msg::String*>(msg));
            std::cout << "Received message: " << string_msg.data() << std::endl;
        }
    });
    
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}