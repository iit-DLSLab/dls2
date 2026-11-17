#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/BaseStatePubSubTypes.hpp"
#include "dls_messages/dds/BlindStatePubSubTypes.hpp"
#include "dls_messages/dds/ros2_interface/sensor_msgs/msg/JointStatePubSubTypes.hpp"
#include "dls_messages/dds/ros2_interface/tf2_msgs/msg/TFMessagePubSubTypes.hpp"

#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace
{
const std::array<std::string, 12> kFallbackJointNames = {
    "lf_haa_joint", "lf_hfe_joint", "lf_kfe_joint",
    "rf_haa_joint", "rf_hfe_joint", "rf_kfe_joint",
    "lh_haa_joint", "lh_hfe_joint", "lh_kfe_joint",
    "rh_haa_joint", "rh_hfe_joint", "rh_kfe_joint"};

constexpr auto kPublishPeriod = std::chrono::milliseconds(20);

uint64_t now_nanoseconds()
{
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
}

void set_stamp_from_nanoseconds(std_msgs::msg::Header& header, const uint64_t timestamp_ns)
{
    const uint64_t timestamp = timestamp_ns > 0 ? timestamp_ns : now_nanoseconds();
    header.stamp().sec(static_cast<int32_t>(timestamp / 1000000000ULL));
    header.stamp().nanosec(static_cast<uint32_t>(timestamp % 1000000000ULL));
}

void set_stamp_from_seconds(std_msgs::msg::Header& header, const double timestamp_seconds)
{
    if (timestamp_seconds <= 0.0)
    {
        set_stamp_from_nanoseconds(header, now_nanoseconds());
        return;
    }

    set_stamp_from_nanoseconds(
        header,
        static_cast<uint64_t>(timestamp_seconds * 1000000000.0));
}

void resize_or_clear(
    std::vector<double>& destination,
    const std::vector<double>& source,
    const std::size_t size)
{
    if (source.size() == size)
    {
        destination = source;
    }
    else
    {
        destination.assign(size, 0.0);
    }
}

bool is_pegasus_joint_name(const std::string& name)
{
    for (const auto& joint_name : kFallbackJointNames)
    {
        if (name == joint_name)
        {
            return true;
        }
    }

    return false;
}

std::optional<std::string> normalize_pegasus_joint_name(const std::string& name)
{
    if (is_pegasus_joint_name(name))
    {
        return name;
    }

    const std::string joint_suffix_name = name + "_joint";
    if (is_pegasus_joint_name(joint_suffix_name))
    {
        return joint_suffix_name;
    }

    return std::nullopt;
}

std::vector<std::string> map_joint_names(const dls2_interface::msg::BlindState& blind_state)
{
    if (blind_state.joints_name().size() == kFallbackJointNames.size())
    {
        std::vector<std::string> mapped_names;
        mapped_names.reserve(blind_state.joints_name().size());

        for (const auto& name : blind_state.joints_name())
        {
            auto mapped_name = normalize_pegasus_joint_name(name);
            if (!mapped_name)
            {
                mapped_names.clear();
                break;
            }
            mapped_names.push_back(*mapped_name);
        }

        if (!mapped_names.empty())
        {
            return mapped_names;
        }
    }

    if (blind_state.joints_position().size() == kFallbackJointNames.size())
    {
        return std::vector<std::string>(kFallbackJointNames.begin(), kFallbackJointNames.end());
    }

    return blind_state.joints_name();
}
}

class RVIZVisualizerInterface
{
public:
    RVIZVisualizerInterface(
        dls::DDSParticipant& participant,
        std::string fixed_frame = "map",
        std::string base_frame = "base_link")
        : fixed_frame_(std::move(fixed_frame))
        , base_frame_(std::move(base_frame))
    {
        joint_state_writer_ = participant.addWriter(
            "rviz_joint_state_writer",
            dls::topicType(
                "joint_states",
                new sensor_msgs::msg::JointStatePubSubType()));

        tf_writer_ = participant.addWriter(
            "rviz_tf_writer",
            dls::topicType(
                "tf",
                new tf2_msgs::msg::TFMessagePubSubType()));

        tf_message_.transforms().resize(1);
        tf_message_.transforms()[0].header().frame_id(fixed_frame_);
        tf_message_.transforms()[0].child_frame_id(base_frame_);

        participant.addReader(
            "rviz_blind_state_reader",
            dls::topicType(
                "blind_state",
                new dls2_interface::msg::BlindStatePubSubType()),
            std::function<void(void*)>{
                [this](void* msg)
                {
                    on_blind_state(*static_cast<dls2_interface::msg::BlindState*>(msg));
                }},
            eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT,
            true);

        participant.addReader(
            "rviz_base_state_reader",
            dls::topicType(
                "base_state",
                new dls2_interface::msg::BaseStatePubSubType()),
            std::function<void(void*)>{
                [this](void* msg)
                {
                    on_base_state(*static_cast<dls2_interface::msg::BaseState*>(msg));
                }},
            eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT,
            true);
    }


    bool run()
    {
        dls2_interface::msg::BlindState blind_state;
        dls2_interface::msg::BaseState base_state;

        {
            std::lock_guard<std::mutex> lock(state_mutex_);
            if (!has_blind_state_ || !has_base_state_)
            {
                return false;
            }

            blind_state = latest_blind_state_;
            base_state = latest_base_state_;
        }

        map_blind_state(blind_state);
        map_base_state(base_state);

        joint_state_writer_->write(&joint_state_);
        tf_writer_->write(&tf_message_);
        return true;
    }

    const sensor_msgs::msg::JointState& joint_state() const
    {
        return joint_state_;
    }

    const tf2_msgs::msg::TFMessage& tf_message() const
    {
        return tf_message_;
    }

private:
    void on_blind_state(const dls2_interface::msg::BlindState& blind_state)
    {
        std::lock_guard<std::mutex> lock(state_mutex_);
        latest_blind_state_ = blind_state;
        has_blind_state_ = true;
    }

    void on_base_state(const dls2_interface::msg::BaseState& base_state)
    {
        std::lock_guard<std::mutex> lock(state_mutex_);
        latest_base_state_ = base_state;
        has_base_state_ = true;
    }

    void map_blind_state(const dls2_interface::msg::BlindState& blind_state)
    {
        joint_state_.header().frame_id(
            blind_state.frame_id().empty() ? base_frame_ : blind_state.frame_id());
        set_stamp_from_seconds(joint_state_.header(), blind_state.timestamp());

        auto mapped_names = map_joint_names(blind_state);
        if (!mapped_names.empty())
        {
            joint_state_.name() = std::move(mapped_names);
        }
        else
        {
            joint_state_.name().assign(kFallbackJointNames.begin(), kFallbackJointNames.end());
        }

        const std::size_t joint_count = joint_state_.name().size();
        resize_or_clear(joint_state_.position(), blind_state.joints_position(), joint_count);
        resize_or_clear(joint_state_.velocity(), blind_state.joints_velocity(), joint_count);
        resize_or_clear(joint_state_.effort(), blind_state.joints_effort(), joint_count);
    }

    void map_base_state(const dls2_interface::msg::BaseState& base_state)
    {
        auto& transform_stamped = tf_message_.transforms()[0];
        transform_stamped.header().frame_id(
            base_state.header().frame_id().empty() ? fixed_frame_ : base_state.header().frame_id());
        transform_stamped.child_frame_id(base_frame_);
        set_stamp_from_nanoseconds(transform_stamped.header(), base_state.header().timestamp());

        const auto& position = base_state.pose().position();
        const auto& orientation = base_state.pose().orientation();

        transform_stamped.transform().translation().x(position[0]);
        transform_stamped.transform().translation().y(position[1]);
        transform_stamped.transform().translation().z(position[2]);

        transform_stamped.transform().rotation().x(orientation[0]);
        transform_stamped.transform().rotation().y(orientation[1]);
        transform_stamped.transform().rotation().z(orientation[2]);
        transform_stamped.transform().rotation().w(orientation[3]);
    }

    std::string fixed_frame_;
    std::string base_frame_;
    eprosima::fastdds::dds::DataWriter* joint_state_writer_{nullptr};
    eprosima::fastdds::dds::DataWriter* tf_writer_{nullptr};
    mutable std::mutex state_mutex_;
    dls2_interface::msg::BlindState latest_blind_state_;
    dls2_interface::msg::BaseState latest_base_state_;
    bool has_blind_state_{false};
    bool has_base_state_{false};
    sensor_msgs::msg::JointState joint_state_;
    tf2_msgs::msg::TFMessage tf_message_;
};

int main()
{
    std::cout << "Starting RViz Visualizer Interface..." << std::endl;

    dls::DDSParticipant participant("rviz_visualizer_participant", 3);
    RVIZVisualizerInterface visualizer(participant);

    std::cout << "Reading BlindState from blind_state and BaseState from base_state" << std::endl;
    std::cout << "Publishing mapped ROS2 joint_states and tf at "
              << 1000 / kPublishPeriod.count() << " Hz when both inputs are available"
              << std::endl;

    int count = 0;
    int wait_count = 0;
    while (true)
    {
        if (visualizer.run())
        {
            if (count % 50 == 0)
            {
                const auto& joint_state = visualizer.joint_state();
                const auto& base_transform = visualizer.tf_message().transforms()[0].transform();
                std::cout << "Published visualizer state: "
                          << joint_state.name().size() << " joints"
                          << " | first joint="
                          << (joint_state.name().empty() ? "<none>" : joint_state.name()[0])
                          << " | base_link=("
                          << base_transform.translation().x() << ", "
                          << base_transform.translation().y() << ", "
                          << base_transform.translation().z() << ")"
                          << std::endl;
            }
            ++count;
        }
        else
        {
            if (wait_count % 50 == 0)
            {
                std::cout << "Waiting for blind_state and base_state" << std::endl;
            }
            ++wait_count;
        }

        std::this_thread::sleep_for(kPublishPeriod);
    }

    return 0;
}
