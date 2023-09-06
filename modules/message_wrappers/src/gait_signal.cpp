#include "dls2/msg_wrappers/gait_signal.hpp"

GaitSignal::GaitSignal(const std::shared_ptr<robotlib::RobotBase> robot) 
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
    , stance_legs_(robot->makeLegDataMap<bool>(false))
{}

GaitSignal::GaitSignal(GaitSignal& gait_signal)
    : frame_id_(gait_signal.frame_id_)
	, sequence_id_(gait_signal.sequence_id_)
	, timestamp_(gait_signal.timestamp_)
    , desired_com_pose_world_(gait_signal.desired_com_pose_world_)
    , desired_com_velocity_world_(gait_signal.desired_com_velocity_world_)
    , desired_com_acceleration_world_(gait_signal.desired_com_acceleration_world_)
	, desired_joints_position_(gait_signal.desired_joints_position_)
    , desired_joints_velocity_(gait_signal.desired_joints_velocity_)
    , desired_joints_acceleration_(gait_signal.desired_joints_acceleration_)
    , desired_joints_effort_(gait_signal.desired_joints_effort_)
	, stance_legs_(gait_signal.stance_legs_)
{}

GaitSignal::~GaitSignal(){}

GaitSignal::operator GaitSignalMsg() const
{
    GaitSignalMsg gait_signal_msg;

	gait_signal_msg.frame_id(frame_id_);
	gait_signal_msg.sequence_id(sequence_id_);
	gait_signal_msg.timestamp(timestamp_);

    const double *p = desired_com_pose_world_.toQuaternion().coeffs().data();
	std::copy(p, p + 4, gait_signal_msg.com_orientation().begin());
    
    for(unsigned int i{0}; i<3; i++)
    {
        gait_signal_msg.com_position()[i] = desired_com_pose_world_.toPosition()[i];
        gait_signal_msg.com_linear_velocity()[i] = desired_com_velocity_world_.getLinear()[i];
        gait_signal_msg.com_angular_velocity()[i] = desired_com_velocity_world_.getAngular()[i];
        gait_signal_msg.com_linear_acceleration()[i] = desired_com_acceleration_world_.getLinear()[i];
        gait_signal_msg.com_angular_acceleration()[i] = desired_com_acceleration_world_.getAngular()[i];
    }

    int i{0};
	for(auto &leg_pair : desired_joints_position_)
	{
		for(auto &joint : *leg_pair.data_)
        {
            gait_signal_msg.joints_position()[i] = desired_joints_position_[joint.key_];
            gait_signal_msg.joints_velocity()[i] = desired_joints_velocity_[joint.key_];
            gait_signal_msg.joints_acceleration()[i] = desired_joints_acceleration_[joint.key_];
            gait_signal_msg.joints_effort()[i] = desired_joints_effort_[joint.key_];
            i++;
        }
    }

    i = 0;
    for(auto &leg_pair : stance_legs_)
	{
    	gait_signal_msg.stance_legs()[i] = *leg_pair.data_;
        i++;
    }

    return gait_signal_msg;
}

GaitSignal& GaitSignal::operator=(const GaitSignalMsg &gait_signal_msg)
{
	frame_id_ = gait_signal_msg.frame_id();
	sequence_id_ = gait_signal_msg.sequence_id();
	timestamp_ = gait_signal_msg.timestamp();

    desired_com_pose_world_.set(Eigen::Vector3d(gait_signal_msg.com_position().data()), Eigen::Quaterniond(gait_signal_msg.com_orientation().data())),
    desired_com_velocity_world_.setLinear(Eigen::Vector3d(gait_signal_msg.com_linear_velocity().data()));
    desired_com_velocity_world_.setAngular(Eigen::Vector3d(gait_signal_msg.com_angular_velocity().data()));
    desired_com_acceleration_world_.setLinear(Eigen::Vector3d(gait_signal_msg.com_linear_acceleration().data()));
    desired_com_acceleration_world_.setAngular(Eigen::Vector3d(gait_signal_msg.com_angular_acceleration().data()));

    int i{0};
    for(auto &leg_pair : desired_joints_position_)
	{
		for(auto &joint : *leg_pair.data_)
        {
            desired_joints_position_[joint.key_] = gait_signal_msg.joints_position()[i];
            desired_joints_velocity_[joint.key_] = gait_signal_msg.joints_velocity()[i];
            desired_joints_acceleration_[joint.key_] = gait_signal_msg.joints_acceleration()[i];
            desired_joints_effort_[joint.key_] = gait_signal_msg.joints_effort()[i];
            i++;
        }
    }

    i = 0;
    for(auto &leg_pair : stance_legs_)
	{
    	*leg_pair.data_ = gait_signal_msg.stance_legs()[i];
        i++;
    }
	
    return *this;
}

GaitSignal& GaitSignal::operator=(const GaitSignal& gait_signal)
{
    frame_id_ = gait_signal.frame_id_;
	sequence_id_ = gait_signal.sequence_id_;
	timestamp_ = gait_signal.timestamp_;

    desired_com_pose_world_ = gait_signal.desired_com_pose_world_;
    desired_com_velocity_world_ = gait_signal.desired_com_velocity_world_;
    desired_com_acceleration_world_ = gait_signal.desired_com_acceleration_world_;

	desired_joints_position_ = gait_signal.desired_joints_position_;
    desired_joints_velocity_ = gait_signal.desired_joints_velocity_;
    desired_joints_acceleration_ = gait_signal.desired_joints_acceleration_;
    desired_joints_effort_ = gait_signal.desired_joints_effort_;
	stance_legs_ = gait_signal.stance_legs_;

	return *this;
}