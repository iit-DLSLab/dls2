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

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "dls2_interfaces/msg/robot_state.hpp"

using std::placeholders::_1;

class MinimalSubscriber : public rclcpp::Node
{
public:
  MinimalSubscriber()
  : Node("minimal_subscriber")
  {
    subscription_ = this->create_subscription<dls2_interfaces::msg::RobotState>(
      "chatter", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
  }

private:
  void topic_callback(const dls2_interfaces::msg::RobotState::SharedPtr msg) const
  {      
    std::cout << "Received message: " << msg->frame_id << std::endl;
    std::cout << "Sequence ID: " << msg->sequence_id << std::endl;
    std::cout << "Timestamp: " << msg->timestamp << std::endl;
    std::cout << "Position: ";
    for (const auto& pos : msg->position)
    {
      std::cout << pos << " ";
    }
    std::cout << std::endl;
    std::cout << "Orientation: ";
    for (const auto& orient : msg->orientation)
    {
      std::cout << orient << " ";
    }
    std::cout << std::endl;
    std::cout << "Joint Positions: ";
    for (const auto& joint_pos : msg->joint_pos)
    {
      std::cout << joint_pos << " ";
    }
    std::cout << std::endl;    
    std::cout << "Test Num: " << msg->test.num << std::endl;
    std::cout << "Joints Temperature: ";
    for (const auto& temp : msg->joints_temperature)
    {
      std::cout << temp << " ";
    }
    std::cout << std::endl;

  }
  rclcpp::Subscription<dls2_interfaces::msg::RobotState>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
