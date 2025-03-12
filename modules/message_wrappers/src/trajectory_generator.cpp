#include "dls2/msg_wrappers/trajectory_generator.hpp"

TrajectoryGenerator::TrajectoryGenerator(const std::shared_ptr<robotlib::RobotBase> robot) 
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
    , desired_com_pose_world_(Eigen::Vector3d::Zero(), Eigen::Quaterniond::Identity())
    , desired_com_velocity_world_(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero())
    , desired_com_acceleration_world_(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero())
    , desired_joints_position_(robot->makeJointState())
    , desired_joints_velocity_(robot->makeJointState())
    , desired_joints_acceleration_(robot->makeJointState())
    , desired_joints_effort_(robot->makeJointState())
    , desired_wrench_(Eigen::Matrix<double, 6, 1>::Zero())
    , stance_legs_(robot->makeLegDataMap<bool>(false))
    , nominal_touch_down_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , touch_down_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , swing_period_(robot->makeLegDataMap<double>(0.0))
    , normal_force_max_(robot->makeLegDataMap<double>(0.0))
    , normal_force_min_(robot->makeLegDataMap<double>(0.0))
    , kp_(robot->makeJointState())
    , kd_(robot->makeJointState())
{}

TrajectoryGenerator::TrajectoryGenerator(const TrajectoryGenerator& trajectory_generator)
    : frame_id_(trajectory_generator.frame_id_)
	, sequence_id_(trajectory_generator.sequence_id_)
	, timestamp_(trajectory_generator.timestamp_)
    , desired_com_pose_world_(trajectory_generator.desired_com_pose_world_)
    , desired_com_velocity_world_(trajectory_generator.desired_com_velocity_world_)
    , desired_com_acceleration_world_(trajectory_generator.desired_com_acceleration_world_)
	, desired_joints_position_(trajectory_generator.desired_joints_position_)
    , desired_joints_velocity_(trajectory_generator.desired_joints_velocity_)
    , desired_joints_acceleration_(trajectory_generator.desired_joints_acceleration_)
    , desired_joints_effort_(trajectory_generator.desired_joints_effort_)
    , desired_wrench_(trajectory_generator.desired_wrench_)
	, stance_legs_(trajectory_generator.stance_legs_)
    , nominal_touch_down_(trajectory_generator.nominal_touch_down_)
    , touch_down_(trajectory_generator.touch_down_)
    , swing_period_(trajectory_generator.swing_period_)
    , normal_force_max_(trajectory_generator.normal_force_max_)
    , normal_force_min_(trajectory_generator.normal_force_min_)
    , kp_(trajectory_generator.kp_)
    , kd_(trajectory_generator.kd_)
{}

TrajectoryGenerator::~TrajectoryGenerator(){}

TrajectoryGenerator::operator TrajectoryGeneratorMsg() const
{
    TrajectoryGeneratorMsg trajectory_generator_msg;

	trajectory_generator_msg.frame_id(frame_id_);
	trajectory_generator_msg.sequence_id(sequence_id_);
	trajectory_generator_msg.timestamp(timestamp_);

    const double *p = desired_com_pose_world_.toQuaternion().coeffs().data();
	std::copy(p, p + 4, trajectory_generator_msg.com_orientation().begin());
    
    for(unsigned int i{0}; i<3; i++)
    {
        trajectory_generator_msg.com_position()[i] = desired_com_pose_world_.toPosition()[i];
        trajectory_generator_msg.com_linear_velocity()[i] = desired_com_velocity_world_.getLinear()[i];
        trajectory_generator_msg.com_angular_velocity()[i] = desired_com_velocity_world_.getAngular()[i];
        trajectory_generator_msg.com_linear_acceleration()[i] = desired_com_acceleration_world_.getLinear()[i];
        trajectory_generator_msg.com_angular_acceleration()[i] = desired_com_acceleration_world_.getAngular()[i];
    }

    int i{0};
    int legs_id{0};
	for(auto &leg_pair : desired_joints_position_)
	{
		for(auto &joint : *leg_pair.data_)
        {
            trajectory_generator_msg.joints_position()[i] = desired_joints_position_[joint.key_];
            trajectory_generator_msg.joints_velocity()[i] = desired_joints_velocity_[joint.key_];
            trajectory_generator_msg.joints_acceleration()[i] = desired_joints_acceleration_[joint.key_];
            trajectory_generator_msg.joints_effort()[i] = desired_joints_effort_[joint.key_];
            trajectory_generator_msg.kp()[i] = kp_[joint.key_];
            trajectory_generator_msg.kd()[i] = kd_[joint.key_];
            i++;
        }
        trajectory_generator_msg.touch_down()[legs_id*3] = touch_down_[leg_pair.key_][0];
        trajectory_generator_msg.touch_down()[legs_id*3 + 1] = touch_down_[leg_pair.key_][1];
        trajectory_generator_msg.touch_down()[legs_id*3 + 2] = touch_down_[leg_pair.key_][2];

        trajectory_generator_msg.swing_period()[legs_id] = swing_period_[leg_pair.key_];

        trajectory_generator_msg.normal_force_max()[legs_id] = normal_force_max_[leg_pair.key_];
        trajectory_generator_msg.normal_force_min()[legs_id] = normal_force_min_[leg_pair.key_];

    	trajectory_generator_msg.stance_legs()[legs_id] = stance_legs_[leg_pair.key_];

        legs_id++;
    }
    
    for(int i=0; i<desired_wrench_.size(); i++)
    {
        trajectory_generator_msg.wrench()[i] = desired_wrench_(i);
    }

    return trajectory_generator_msg;
}

TrajectoryGenerator& TrajectoryGenerator::operator=(const TrajectoryGeneratorMsg &trajectory_generator_msg)
{
	frame_id_ = trajectory_generator_msg.frame_id();
	sequence_id_ = trajectory_generator_msg.sequence_id();
	timestamp_ = trajectory_generator_msg.timestamp();

    desired_com_pose_world_.set(Eigen::Vector3d(trajectory_generator_msg.com_position().data()), Eigen::Quaterniond(trajectory_generator_msg.com_orientation().data())),
    desired_com_velocity_world_.setLinear(Eigen::Vector3d(trajectory_generator_msg.com_linear_velocity().data()));
    desired_com_velocity_world_.setAngular(Eigen::Vector3d(trajectory_generator_msg.com_angular_velocity().data()));
    desired_com_acceleration_world_.setLinear(Eigen::Vector3d(trajectory_generator_msg.com_linear_acceleration().data()));
    desired_com_acceleration_world_.setAngular(Eigen::Vector3d(trajectory_generator_msg.com_angular_acceleration().data()));

    int i{0};
    int legs_id{0};
    for(auto &leg_pair : desired_joints_position_)
	{
		for(auto &joint : *leg_pair.data_)
        {
            desired_joints_position_[joint.key_] = trajectory_generator_msg.joints_position()[i];
            desired_joints_velocity_[joint.key_] = trajectory_generator_msg.joints_velocity()[i];
            desired_joints_acceleration_[joint.key_] = trajectory_generator_msg.joints_acceleration()[i];
            desired_joints_effort_[joint.key_] = trajectory_generator_msg.joints_effort()[i];
            kp_[joint.key_] = trajectory_generator_msg.kp()[i];
            kd_[joint.key_] = trajectory_generator_msg.kd()[i];

            i++;
        }
        touch_down_[leg_pair.key_][0] = trajectory_generator_msg.touch_down()[legs_id*3];
        touch_down_[leg_pair.key_][1] = trajectory_generator_msg.touch_down()[legs_id*3 + 1];
        touch_down_[leg_pair.key_][2] = trajectory_generator_msg.touch_down()[legs_id*3 + 2];

        swing_period_[leg_pair.key_] = trajectory_generator_msg.swing_period()[legs_id];

        normal_force_max_[leg_pair.key_] = trajectory_generator_msg.normal_force_max()[legs_id];
        normal_force_min_[leg_pair.key_] = trajectory_generator_msg.normal_force_min()[legs_id];

    	stance_legs_[leg_pair.key_] = trajectory_generator_msg.stance_legs()[legs_id];
        legs_id++;
    }
	
    for(int i=0; i<desired_wrench_.size(); i++)
    {
        desired_wrench_(i) = trajectory_generator_msg.wrench()[i];
    }

    return *this;
}

TrajectoryGenerator& TrajectoryGenerator::operator=(const TrajectoryGenerator& trajectory_generator)
{
    frame_id_ = trajectory_generator.frame_id_;
	sequence_id_ = trajectory_generator.sequence_id_;
	timestamp_ = trajectory_generator.timestamp_;

    desired_com_pose_world_ = trajectory_generator.desired_com_pose_world_;
    desired_com_velocity_world_ = trajectory_generator.desired_com_velocity_world_;
    desired_com_acceleration_world_ = trajectory_generator.desired_com_acceleration_world_;

	desired_joints_position_ = trajectory_generator.desired_joints_position_;
    desired_joints_velocity_ = trajectory_generator.desired_joints_velocity_;
    desired_joints_acceleration_ = trajectory_generator.desired_joints_acceleration_;
    desired_joints_effort_ = trajectory_generator.desired_joints_effort_;
    desired_wrench_ = trajectory_generator.desired_wrench_;
	kp_ = trajectory_generator.kp_;
    kd_ = trajectory_generator.kd_;

	stance_legs_ = trajectory_generator.stance_legs_;

    nominal_touch_down_ = trajectory_generator.nominal_touch_down_;
    touch_down_ = trajectory_generator.touch_down_;
    swing_period_ = trajectory_generator.swing_period_;

    normal_force_max_ = trajectory_generator.normal_force_max_;
    normal_force_min_ = trajectory_generator.normal_force_min_;

	return *this;
}

void TrajectoryGenerator::setDataFromWrapperBase(WrapperBase* wrapper_base)
{
    // Assuming wrapper is of type Wrapper<MsgType>
    *this = static_cast<TrajectoryGenerator&>(*wrapper_base);
}