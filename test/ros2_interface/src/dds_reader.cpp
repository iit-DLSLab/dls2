#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/ros2_interface/sensor_msgs/msg/ImagePubSubTypes.hpp"

#include <thread>
#include <any>

int main()
{
    dls::DDSParticipant participant("participant", 3);
    participant.addReader("dds_reader", dls::topicType("image", new sensor_msgs::msg::ImagePubSubType()), std::function<void(void *)>
    {
        [&](void *msg)
        {
            sensor_msgs::msg::Image image_msg = *(static_cast<sensor_msgs::msg::Image*>(msg));
            std::cout << "\nReceived Image message: " << std::endl;
            std::cout << "Header Frame ID: " << image_msg.header().frame_id() << std::endl;
            std::cout << "Header Timestamp: " << image_msg.header().stamp().sec() << " sec, " << image_msg.header().stamp().nanosec() << " nanosec" << std::endl;
            std::cout << "Image Height: " << image_msg.height() << std::endl;
            std::cout << "Image Width: " << image_msg.width() << std::endl;
            std::cout << "Image Encoding: " << image_msg.encoding() << std::endl;
            std::cout << "Is Big Endian: " << static_cast<int>(image_msg.is_bigendian()) << std::endl;
            // print step and data
            std::cout << "Image Step: " << image_msg.step() << std::endl;
            std::cout << "Image Data Size: " << image_msg.data().size() << std::endl;
            // print data values (first 10 values for brevity)
            std::cout << "Image Data (first 10 values): ";
            for (size_t i = 0; i < std::min(image_msg.data().size(), static_cast<size_t>(10)); ++i)
            {
                std::cout << static_cast<int>(image_msg.data()[i]) << " ";
            }
        }
    });
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}
