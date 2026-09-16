#include "dls2/util/waypointer.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace dls
{  
    namespace utils
    {
        bool Waypointer::init(const std::vector<Waypoint>& path){
            if(path.empty() 
            || !std::all_of(path.begin(), path.end(), [](const Waypoint& point) {
                return std::isfinite(point.first) && std::isfinite(point.second);
                }))
            {
                path_.clear();
                return false;
            }
            path_ = path;

            return true;
        }

        EuclideanWaypointer::EuclideanWaypointer(std::string config_path)
        : Waypointer()
        {
            auto config = YAML::LoadFile(config_path);
            monotonic_ = config["monotonic"].as<bool>();
            const auto lookahead = config["lookahead"].as<int>();
            if (lookahead < 0) {
                throw std::invalid_argument("waypointer lookahead must be nonnegative");
            }
            lookahead_ = static_cast<size_t>(lookahead);
            panic_threshold_ = config["panic_threshold"].as<double>();
        }

        EuclideanWaypointer::EuclideanWaypointer(const YAML::Node& config)
        : Waypointer()
        {
            monotonic_ = config["monotonic"].as<bool>();
            const auto lookahead = config["lookahead"].as<int>();
            if (lookahead < 0) {
                throw std::invalid_argument("waypointer lookahead must be nonnegative");
            }
            lookahead_ = static_cast<size_t>(lookahead);
            panic_threshold_ = config["panic_threshold"].as<double>();
        }

        bool EuclideanWaypointer::init(const std::vector<Waypoint>& path){
            panic_ = false;
            last_min_dist_idx_ = 0;
            return Waypointer::init(path);
        }

        std::pair<double, size_t> EuclideanWaypointer::closestWaypoint(const Waypoint& robot_pose){

            double min_dist = std::numeric_limits<double>::infinity();
            size_t min_dist_idx = path_.size() - 1;
            size_t start_idx = (monotonic_ && !panic_) ? last_min_dist_idx_ : 0;

            for(size_t i = start_idx; i < path_.size(); i++){
                const auto [x, y] = path_.at(i);
                const auto delta_x = x - robot_pose.first;
                const auto delta_y = y - robot_pose.second;
                const auto euclidean_distance = sqrt(delta_x * delta_x + delta_y * delta_y);

                if(euclidean_distance < min_dist){
                    min_dist = euclidean_distance;
                    min_dist_idx = i;
                }
            }

            
            std::pair<double, size_t> best {min_dist, min_dist_idx};
            return best; 
        }


        bool EuclideanWaypointer::run(const Waypoint& robot_pose, Waypoint& waypoint){
            if(path_.empty() || 
                !std::isfinite(robot_pose.first) ||
                !std::isfinite(robot_pose.second))
            {
                return false;
            }

            auto [min_dist, min_dist_idx] = closestWaypoint(robot_pose);

            if(min_dist > panic_threshold_){
                panic_ = true;
                std::tie(min_dist, min_dist_idx) = closestWaypoint(robot_pose);
            }else{
                panic_ = false;
            }

            min_dist_idx = std::min(lookahead_, path_.size() - 1 - min_dist_idx) + min_dist_idx;

            last_min_dist_idx_ = min_dist_idx;
            waypoint = path_.at(min_dist_idx);
            return true;

        }
    } // namespace utils
} // namespace dls
