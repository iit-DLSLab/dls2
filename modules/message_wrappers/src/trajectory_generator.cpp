#include "dls2/msg_wrappers/trajectory_generator.hpp"

using namespace dls;

TrajectoryGenerator::TrajectoryGenerator(const std::shared_ptr<robotlib::RobotBase> robot)
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
    , feet_position_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , feet_position_HF_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , feet_velocity_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , feet_velocity_HF_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , feet_acceleration_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , nominal_touch_down_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , touch_down_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , swing_period_(robot->makeLegDataMap<double>(0.0))
    , normal_force_max_(robot->makeLegDataMap<double>(0.0))
    , normal_force_min_(robot->makeLegDataMap<double>(0.0))
    , stance_(robot->makeLegDataMap<bool>(false))
{}

TrajectoryGenerator::TrajectoryGenerator(const TrajectoryGenerator& trajectory_generator)
    : frame_id_(trajectory_generator.frame_id_)
	, sequence_id_(trajectory_generator.sequence_id_)
	, timestamp_(trajectory_generator.timestamp_)
    , feet_position_(trajectory_generator.feet_position_)
    , feet_position_HF_(trajectory_generator.feet_position_HF_)
    , feet_velocity_(trajectory_generator.feet_velocity_)
    , feet_velocity_HF_(trajectory_generator.feet_velocity_HF_)
    , feet_acceleration_(trajectory_generator.feet_acceleration_)
    , nominal_touch_down_(trajectory_generator.nominal_touch_down_)
    , touch_down_(trajectory_generator.touch_down_)
    , swing_period_(trajectory_generator.swing_period_)
    , normal_force_max_(trajectory_generator.normal_force_max_)
    , normal_force_min_(trajectory_generator.normal_force_min_)
    , stance_(trajectory_generator.stance_)
{}

TrajectoryGenerator::~TrajectoryGenerator(){}

TrajectoryGenerator::operator TrajectoryGeneratorMsg() const
{
    TrajectoryGeneratorMsg trajectory_generator_msg;

	trajectory_generator_msg.frame_id(frame_id_);
	trajectory_generator_msg.sequence_id(sequence_id_);
	trajectory_generator_msg.timestamp(timestamp_);

    int i{0};
    for(auto &leg_pair : feet_position_)
	{
	 	trajectory_generator_msg.feet_position()[i*3] = feet_position_[leg_pair.key_][0];
        trajectory_generator_msg.feet_position()[i*3 + 1] = feet_position_[leg_pair.key_][1];
        trajectory_generator_msg.feet_position()[i*3 + 2] = feet_position_[leg_pair.key_][2];

        trajectory_generator_msg.feet_position_HF()[i*3] = feet_position_HF_[leg_pair.key_][0];
        trajectory_generator_msg.feet_position_HF()[i*3 + 1] = feet_position_HF_[leg_pair.key_][1];
        trajectory_generator_msg.feet_position_HF()[i*3 + 2] = feet_position_HF_[leg_pair.key_][2];

        trajectory_generator_msg.feet_velocity()[i*3] = feet_velocity_[leg_pair.key_][0];
        trajectory_generator_msg.feet_velocity()[i*3 + 1] = feet_velocity_[leg_pair.key_][1];
        trajectory_generator_msg.feet_velocity()[i*3 + 2] = feet_velocity_[leg_pair.key_][2];

        trajectory_generator_msg.feet_velocity_HF()[i*3] = feet_velocity_HF_[leg_pair.key_][0];
        trajectory_generator_msg.feet_velocity_HF()[i*3 + 1] = feet_velocity_HF_[leg_pair.key_][1];
        trajectory_generator_msg.feet_velocity_HF()[i*3 + 2] = feet_velocity_HF_[leg_pair.key_][2];

        trajectory_generator_msg.feet_acceleration()[i*3] = feet_acceleration_[leg_pair.key_][0];
        trajectory_generator_msg.feet_acceleration()[i*3 + 1] = feet_acceleration_[leg_pair.key_][1];
        trajectory_generator_msg.feet_acceleration()[i*3 + 2] = feet_acceleration_[leg_pair.key_][2];

        trajectory_generator_msg.nominal_touch_down()[i*3] = nominal_touch_down_[leg_pair.key_][0];
        trajectory_generator_msg.nominal_touch_down()[i*3 + 1] = nominal_touch_down_[leg_pair.key_][1];
        trajectory_generator_msg.nominal_touch_down()[i*3 + 2] = nominal_touch_down_[leg_pair.key_][2];

        trajectory_generator_msg.touch_down()[i*3] = touch_down_[leg_pair.key_][0];
        trajectory_generator_msg.touch_down()[i*3 + 1] = touch_down_[leg_pair.key_][1];
        trajectory_generator_msg.touch_down()[i*3 + 2] = touch_down_[leg_pair.key_][2];

        trajectory_generator_msg.swing_period()[i] = swing_period_[leg_pair.key_];

        trajectory_generator_msg.normal_force_max()[i] = normal_force_max_[leg_pair.key_];
        trajectory_generator_msg.normal_force_min()[i] = normal_force_min_[leg_pair.key_];

        trajectory_generator_msg.stance()[i] = stance_[leg_pair.key_];
        i++;
    }

    return trajectory_generator_msg;
}

TrajectoryGenerator& TrajectoryGenerator::operator=(const TrajectoryGeneratorMsg& trajectory_generator_msg)
{
	frame_id_ = trajectory_generator_msg.frame_id();
	sequence_id_ = trajectory_generator_msg.sequence_id();
	timestamp_ = trajectory_generator_msg.timestamp();

    int i{0};
    for(auto &leg_pair : feet_position_)
	{
	 	feet_position_[leg_pair.key_][0] = trajectory_generator_msg.feet_position()[i*3];
        feet_position_[leg_pair.key_][1] = trajectory_generator_msg.feet_position()[i*3 + 1];
        feet_position_[leg_pair.key_][2] = trajectory_generator_msg.feet_position()[i*3 + 2];

        feet_position_HF_[leg_pair.key_][0] = trajectory_generator_msg.feet_position_HF()[i*3];
        feet_position_HF_[leg_pair.key_][1] = trajectory_generator_msg.feet_position_HF()[i*3 + 1];
        feet_position_HF_[leg_pair.key_][2] = trajectory_generator_msg.feet_position_HF()[i*3 + 2];

        feet_velocity_[leg_pair.key_][0] = trajectory_generator_msg.feet_velocity()[i*3];
        feet_velocity_[leg_pair.key_][1] = trajectory_generator_msg.feet_velocity()[i*3 + 1];
        feet_velocity_[leg_pair.key_][2] = trajectory_generator_msg.feet_velocity()[i*3 + 2];

        feet_velocity_HF_[leg_pair.key_][0] = trajectory_generator_msg.feet_velocity_HF()[i*3];
        feet_velocity_HF_[leg_pair.key_][1] = trajectory_generator_msg.feet_velocity_HF()[i*3 + 1];
        feet_velocity_HF_[leg_pair.key_][2] = trajectory_generator_msg.feet_velocity_HF()[i*3 + 2];

        feet_acceleration_[leg_pair.key_][0] = trajectory_generator_msg.feet_acceleration()[i*3];
        feet_acceleration_[leg_pair.key_][1] = trajectory_generator_msg.feet_acceleration()[i*3 + 1];
        feet_acceleration_[leg_pair.key_][2] = trajectory_generator_msg.feet_acceleration()[i*3 + 2];

        nominal_touch_down_[leg_pair.key_][0] = trajectory_generator_msg.nominal_touch_down()[i*3];
        nominal_touch_down_[leg_pair.key_][1] = trajectory_generator_msg.nominal_touch_down()[i*3 + 1];
        nominal_touch_down_[leg_pair.key_][2] = trajectory_generator_msg.nominal_touch_down()[i*3 + 2];

        touch_down_[leg_pair.key_][0] = trajectory_generator_msg.touch_down()[i*3];
        touch_down_[leg_pair.key_][1] = trajectory_generator_msg.touch_down()[i*3 + 1];
        touch_down_[leg_pair.key_][2] = trajectory_generator_msg.touch_down()[i*3 + 2];

        swing_period_[leg_pair.key_] = trajectory_generator_msg.swing_period()[i];

        stance_[leg_pair.key_] = trajectory_generator_msg.stance()[i];

        normal_force_max_[leg_pair.key_] = trajectory_generator_msg.normal_force_max()[i];
        normal_force_min_[leg_pair.key_] = trajectory_generator_msg.normal_force_min()[i];
        i++;
    }
    return *this;
}

TrajectoryGenerator& TrajectoryGenerator::operator=(const TrajectoryGenerator& trajectory_generator)
{
    frame_id_ = trajectory_generator.frame_id_;
	sequence_id_ = trajectory_generator.sequence_id_;
	timestamp_ = trajectory_generator.timestamp_;

    feet_position_ = trajectory_generator.feet_position_;
    feet_position_HF_ = trajectory_generator.feet_position_HF_;
    feet_velocity_ = trajectory_generator.feet_velocity_;
    feet_velocity_HF_ = trajectory_generator.feet_velocity_HF_;
    feet_acceleration_ = trajectory_generator.feet_acceleration_;

    nominal_touch_down_ = trajectory_generator.nominal_touch_down_;
    touch_down_ = trajectory_generator.touch_down_;
    swing_period_ = trajectory_generator.swing_period_;

    normal_force_max_ = trajectory_generator.normal_force_max_;
    normal_force_min_ = trajectory_generator.normal_force_min_;

    stance_ = trajectory_generator.stance_;

	return *this;
}