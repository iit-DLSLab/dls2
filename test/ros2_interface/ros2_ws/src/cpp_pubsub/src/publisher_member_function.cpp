// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "dls2_interfaces/msg/robot_state.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<dls2_interfaces::msg::RobotState>("chatter", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto message = dls2_interfaces::msg::RobotState();
    message.frame_id = "writer";
    message.sequence_id = count_;
    message.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    message.position = {1.0 + count_, 2.0 + count_, 3.0 + count_};
    message.orientation = {0.0, 0.0, 0.0, 1.0}; // Example orientation
    message.joint_pos = {1.0 + count_, 2.0 + count_, 3.0 + count_};
    message.test.num = 42.0; // Example test data
    std::cout << "Frame ID: " << message.frame_id << std::endl;
    std::cout << "Sequence ID: " << message.sequence_id << std::endl;
    std::cout << "Timestamp: " << message.timestamp << std::endl;
    std::cout << "Position: ";
    for (const auto& pos : message.position) {
      std::cout << pos << " ";
    }
    std::cout << std::endl;
    std::cout << "Orientation: ";
    for (const auto& orient : message.orientation) {
      std::cout << orient << " ";
    }
    std::cout << std::endl;
    std::cout << "Joint Positions: ";
    for (const auto& joint_pos : message.joint_pos) {
      std::cout << joint_pos << " ";
    }
    std::cout << std::endl;
    std::cout << "Test Num: " << message.test.num << std::endl; // Accessing the Test message
    message.joints_temperature = {30.0 + count_, 31.0 + count_, 32.0 + count_}; // Example temperature data
    count_++;
    publisher_->publish(message);
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<dls2_interfaces::msg::RobotState>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
