#pragma once

#include <vector>
#include <utility>
#include <string>
#include <limits>
#include <tuple>
#include <yaml-cpp/yaml.h>

namespace dls
{
    namespace utils
    {
        using Waypoint = std::pair<double, double>;

        class Waypointer
        {
            public:
                Waypointer() = default;

                bool init(const std::vector<Waypoint>& path);
                virtual bool run(const Waypoint& robot_pose, Waypoint& waypoint) = 0;

            protected:
                std::vector<Waypoint> path_;
        };

        class EuclideanWaypointer : public Waypointer
        {
            public:
                EuclideanWaypointer(std::string config_path);
                EuclideanWaypointer(const YAML::Node& config);
                
                bool init(const std::vector<Waypoint>& path);
                bool run(const Waypoint& robot_pose, Waypoint& waypoint) override;
                std::pair<double, size_t> closestWaypoint(const Waypoint& robot_pose);

            private:
                bool panic_ { false };
                double panic_threshold_ { 10.0 };

                bool monotonic_{ true };
                size_t last_min_dist_idx_ { 0 };
                size_t lookahead_ { 0 };
        };
    } // namespace utils
} // namespace dls
