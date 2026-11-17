#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/ros2_interface/sensor_msgs/msg/ImagePubSubTypes.hpp"

#include <thread>
#include <any>

int main()
{
    dls::DDSParticipant participant("participant", 3);
    auto writer = participant.addWriter("dds_writer", dls::topicType("image", new sensor_msgs::msg::ImagePubSubType()));
    sensor_msgs::msg::Image msg;
    int count = 0;
    while(true)
    {
        writer->write(&msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        count++;
    }
    return 0;
}