#ifndef TRAJECTORY_GENERATOR_HPP
#define TRAJECTORY_GENERATOR_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/trajectory_generator.h"
#include <robotlib/robot_base.hpp>

namespace dls
{
    class  TrajectoryGenerator : public Wrapper<TrajectoryGeneratorMsg>
    {
    public:
        TrajectoryGenerator(const std::shared_ptr<robotlib::RobotBase> robot);
        TrajectoryGenerator(const TrajectoryGenerator& trajectory_generator);
        TrajectoryGenerator() = delete;
        virtual ~TrajectoryGenerator();

        operator TrajectoryGeneratorMsg() const override;
		TrajectoryGenerator& operator=(const TrajectoryGeneratorMsg& trajectory_generator_msg) override;
		TrajectoryGenerator& operator=(const TrajectoryGenerator& trajectory_generator);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

        robotlib::LegDataMap<Eigen::Vector3d> feet_position_;
        robotlib::LegDataMap<Eigen::Vector3d> feet_position_HF_;
        robotlib::LegDataMap<Eigen::Vector3d> feet_velocity_;
        robotlib::LegDataMap<Eigen::Vector3d> feet_velocity_HF_;
        robotlib::LegDataMap<Eigen::Vector3d> feet_acceleration_;

        robotlib::LegDataMap<Eigen::Vector3d> nominal_touch_down_;
        robotlib::LegDataMap<Eigen::Vector3d> touch_down_;
        robotlib::LegDataMap<double> swing_period_;

        robotlib::LegDataMap<double> normal_force_max_;
        robotlib::LegDataMap<double> normal_force_min_;

        robotlib::LegDataMap<bool> stance_;
    };
}
#endif