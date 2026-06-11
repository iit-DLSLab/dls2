#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/ros2_interface/visualization_msgs/msg/MarkerPubSubTypes.hpp"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <thread>

namespace
{
void set_stamp(std_msgs::msg::Header& header)
{
    const auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    const auto sec = now.count() / 1000000000;
    const auto nanosec = now.count() % 1000000000;

    header.stamp().sec(static_cast<int32_t>(sec));
    header.stamp().nanosec(static_cast<uint32_t>(nanosec));
}

void initialize_marker(visualization_msgs::msg::Marker& marker)
{
    marker.header().frame_id("map");
    set_stamp(marker.header());

    marker.ns("dls2_marker_example");
    marker.id(0);
    marker.type(visualization_msgs::msg::SPHERE);
    marker.action(visualization_msgs::msg::ADD);

    marker.pose().position().x(0.0);
    marker.pose().position().y(0.0);
    marker.pose().position().z(0.5);
    marker.pose().orientation().x(0.0);
    marker.pose().orientation().y(0.0);
    marker.pose().orientation().z(0.0);
    marker.pose().orientation().w(1.0);

    marker.scale().x(0.4);
    marker.scale().y(0.4);
    marker.scale().z(0.4);

    marker.color().r(0.0F);
    marker.color().g(0.7F);
    marker.color().b(1.0F);
    marker.color().a(1.0F);

    marker.lifetime().sec(0);
    marker.lifetime().nanosec(0);
    marker.frame_locked(false);

    marker.points().clear();
    marker.colors().clear();
    marker.texture_resource("");
    marker.texture().header().frame_id("");
    marker.texture().header().stamp().sec(0);
    marker.texture().header().stamp().nanosec(0);
    marker.texture().format("");
    marker.texture().data().clear();
    marker.uv_coordinates().clear();
    marker.text("");
    marker.mesh_resource("");
    marker.mesh_file().filename("");
    marker.mesh_file().data().clear();
    marker.mesh_use_embedded_materials(false);
}
}

int main()
{
    std::cout << "Starting DDS Marker Writer..." << std::endl;
    
    dls::DDSParticipant participant("marker_participant", 3);
    auto writer = participant.addWriter(
        "dds_writer_marker",
        dls::topicType(
            "visualization_marker",
            new visualization_msgs::msg::MarkerPubSubType()));

    visualization_msgs::msg::Marker marker;
    initialize_marker(marker);
    int count = 0;
    while (true)
    {
        set_stamp(marker.header());

        const double t = static_cast<double>(count) * 0.1;
        marker.pose().position().x(std::cos(t));
        marker.pose().position().y(std::sin(t));
        marker.pose().position().z(0.5);

        writer->write(&marker);

        std::cout << "Published Marker on visualization_marker: x="
                  << marker.pose().position().x()
                  << " y=" << marker.pose().position().y()
                  << " z=" << marker.pose().position().z()
                  << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ++count;
    }

    return 0;
}
