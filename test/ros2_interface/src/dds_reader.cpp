#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/RobotStatePubSubTypes.hpp"

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
    participant.addReader("dds_reader", dls::topicType("rt/chatter", new dls2_interfaces::msg::RobotStatePubSubType()), std::function<void(void *)>
    {
        [&](void *msg)
        {
            dls2_interfaces::msg::RobotState robot_state_msg = *(static_cast<dls2_interfaces::msg::RobotState*>(msg));
            std::cout << "\nReceived message: " << robot_state_msg.frame_id() << std::endl;
            std::cout << "Sequence ID: " << robot_state_msg.sequence_id() << std::endl;
            std::cout << "Timestamp: " << robot_state_msg.timestamp() << std::endl;
            std::cout << "Position: ";
            for (const auto& pos : robot_state_msg.position())
            {
                std::cout << pos << " ";
            }
            std::cout << std::endl;
            std::cout << "Orientation: ";
            for (const auto& orient : robot_state_msg.orientation())
            {
                std::cout << orient << " ";
            }
            std::cout << std::endl;
            std::cout << "Joint Positions: ";
            for (const auto& joint_pos : robot_state_msg.joint_pos())
            {
                std::cout << joint_pos << " ";
            }
            std::cout << std::endl;
            std::cout << "Test Num: " << robot_state_msg.test().num() << std::endl; // Accessing the Test message

            std::cout << "Joints Temperature: ";
            for (const auto& temp : robot_state_msg.joints_temperature())
            {
                std::cout << temp << " ";
            }
            std::cout << std::endl;
            }
    });
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}