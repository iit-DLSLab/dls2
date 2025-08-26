#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/blind_statePubSubTypes.hpp"
#include "dls_messages/dds/RobotStatePubSubTypes.hpp"

#include <thread>
#include <any>

int main()
{
    dls::DDSParticipant participant("participant", 0);
    auto writer = participant.addWriter("dds_writer", dls::topicType("rt/chatter", new dls2_interfaces::msg::RobotStatePubSubType()));
    dls2_interfaces::msg::RobotState msg;

    int count = 0;
    while(true)
    {
        msg.frame_id() = "writer";
        msg.sequence_id() = count;
        msg.timestamp() = std::chrono::system_clock::now().time_since_epoch().count();
        msg.position() = {1.0 + count, 2.0 + count, 3.0 + count};
        msg.orientation() = {0.0, 0.0, 0.0, 1.0}; // Example orientation
        msg.joint_pos() = {1.0 + count, 2.0 + count, 3.0 + count};
        msg.test().num() = 42.0; // Example test data
        msg.joints_temperature() = {30.0 + count, 31.0 + count, 32.0 + count}; // Example temperature data
        std::cout << "Frame ID: " << msg.frame_id() << std::endl;
        std::cout << "Sequence ID: " << msg.sequence_id() << std::endl;
        std::cout << "Timestamp: " << msg.timestamp() << std::endl;
        std::cout << "Position: ";
        for (const auto& pos : msg.position())
        {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
        std::cout << "Orientation: ";
        for (const auto& orient : msg.orientation())
        {
            std::cout << orient << " ";
        }
        std::cout << std::endl;
        std::cout << "Joint Positions: ";
        for (const auto& joint_pos : msg.joint_pos())
        {
            std::cout << joint_pos << " ";
        }
        std::cout << std::endl;
        std::cout << "Test Num: " << msg.test().num() << std::endl;
        std::cout << "Joints Temperature: ";
        for (const auto& temp : msg.joints_temperature())
        {
            std::cout << temp << " ";
        }
        std::cout << std::endl;
        
        writer->write(&msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        count++;
    }
    return 0;
}