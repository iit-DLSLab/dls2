#ifndef DLS2_UTILS_HPP
#define DLS2_UTILS_HPP

#include <chrono>
#include <functional>
#include <thread>
#include "robotlib/robot_base.hpp"
#include <yaml-cpp/yaml.h>
namespace dls
{
	namespace utils{
        
        bool wait(std::function<bool()> func, int timeout_ms, int period_ms, bool& stop_wait);
        bool wait(std::function<bool()> func, int timeout_ms, int period_ms, std::atomic_bool& stop_wait);
        void readFoldConfiguration(std::shared_ptr<robotlib::RobotBase> robot, robotlib::LegDataMap<Eigen::Vector3d>& feet_pos_des);
        void readHomeConfiguration(std::shared_ptr<robotlib::RobotBase> robot, robotlib::LegDataMap<Eigen::Vector3d>& feet_pos_des);
        void getConfiguration(YAML::Node config, std::shared_ptr<robotlib::RobotBase> robot, robotlib::LegDataMap<Eigen::Vector3d>& feet_pos_des);
    }// namespace utils

} // namespace dls

#endif /* end of include guard: DLS2_UTILS_HPP */
