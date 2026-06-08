#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/RobotStatePubSubTypes.hpp"
#include "dls_messages/dds/ImagePubSubTypes.hpp"

#include <thread>
#include <any>

// 3 main steps to connect FastDDS to ROS2
// 1. generate .idl from .msg file:
//      rosidl translate --to idl -o . package_name message_name.msg
// 2. generate fastdds message using the -typeros2 option (-cs is used to allow case-sentitive names): 
//      fastddsgen -replace -cs -typeros2 message_name.idl
// 3. create a fastdds reader/writer connected to the ROS 2 topic name.
// NB: a reader/writer could be configured to interface with ROS2. This means that the rt/ is automatically added before the topic name

int main()
{
    dls::DDSParticipant participant("participant", 3);
    // participant.addReader("dds_reader", dls::topicType("chatter", new dls2_interfaces::msg::RobotStatePubSubType()), std::function<void(void *)>
    participant.addReader("dds_reader", dls::topicType("image", new sensor_msgs::msg::ImagePubSubType()), std::function<void(void *)>
    {
        [&](void *msg)
        {
            // dls2_interfaces::msg::RobotState robot_state_msg = *(static_cast<dls2_interfaces::msg::RobotState*>(msg));
            // std::cout << "\nReceived message: " << robot_state_msg.frame_id() << std::endl;
            // std::cout << "Sequence ID: " << robot_state_msg.sequence_id() << std::endl;
            // std::cout << "Timestamp: " << robot_state_msg.timestamp() << std::endl;
            // std::cout << "Position: ";
            // for (const auto& pos : robot_state_msg.position())
            // {
            //     std::cout << pos << " ";
            // }
            // std::cout << std::endl;
            // std::cout << "Orientation: ";
            // for (const auto& orient : robot_state_msg.orientation())
            // {
            //     std::cout << orient << " ";
            // }
            // std::cout << std::endl;
            // std::cout << "Joint Positions: ";
            // for (const auto& joint_pos : robot_state_msg.joint_pos())
            // {
            //     std::cout << joint_pos << " ";
            // }
            // std::cout << std::endl;
            // std::cout << "Test Num: " << robot_state_msg.test().num() << std::endl; // Accessing the Test message

            // std::cout << "Joints Temperature: ";
            // for (const auto& temp : robot_state_msg.joints_temperature())
            // {
            //     std::cout << temp << " ";
            // }
            // std::cout << std::endl;
            // }
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
