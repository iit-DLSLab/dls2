#include "dls2/msg_wrappers/blind_state.hpp"

BlindState::BlindState(const std::shared_ptr<robotlib::RobotBase> robot)
	: frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, robot_name_(robot->getName())
	, joints_name_(robot->makeJointDataMap<std::string>(""))
	, joints_position_(robot->makeJointState())
	, joints_velocity_(robot->makeJointState())
	, joints_acceleration_(robot->makeJointState())
	, joints_effort_(robot->makeJointState())
	, joints_temperature_(robot->makeJointState())
	, feet_position_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, feet_velocity_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, feet_acceleration_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, terrain_inclination_(Eigen::Vector3d::Zero())
	, surface_normal_(Eigen::Vector3d::Zero())
	, base_pose_world_(Eigen::Vector3d::Zero(), Eigen::Quaterniond::Identity())
	, base_orientation_world_rpy_(Eigen::Vector3d::Zero())
	, base_velocity_world_(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero())
	, base_acceleration_world_(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero())
	, stance_legs_(robot->makeLegDataMap<bool>(false))
{}

BlindState::BlindState(BlindState& blind_state)
	: frame_id_(blind_state.frame_id_)
	, sequence_id_(blind_state.sequence_id_)
	, timestamp_(blind_state.timestamp_)
	, robot_name_(blind_state.robot_name_)
	, joints_name_(blind_state.joints_name_) 
	, joints_position_(blind_state.joints_position_)
	, joints_velocity_(blind_state.joints_velocity_)
	, joints_acceleration_(blind_state.joints_acceleration_)
	, joints_effort_(blind_state.joints_effort_)
	, joints_temperature_(blind_state.joints_temperature_)
	, feet_position_(blind_state.feet_position_)
	, feet_velocity_(blind_state.feet_velocity_)
	, feet_acceleration_(blind_state.feet_acceleration_)
	, terrain_inclination_(blind_state.terrain_inclination_)
	, surface_normal_(blind_state.surface_normal_)
	, base_pose_world_(blind_state.base_pose_world_)
	, base_orientation_world_rpy_(blind_state.base_orientation_world_rpy_)
	, base_velocity_world_(blind_state.base_velocity_world_)
	, base_acceleration_world_(blind_state.base_acceleration_world_)
	, stance_legs_(blind_state.stance_legs_)
{}

BlindState::~BlindState(){}

BlindState::operator BlindStateMsg() const
{
    BlindStateMsg blind_state_msg;

	blind_state_msg.frame_id(frame_id_);
	blind_state_msg.sequence_id(sequence_id_);
	blind_state_msg.timestamp(timestamp_);

	blind_state_msg.robot_name(robot_name_);

	int leg_id{0};
	int leg_joint_id{0};

	for(auto &leg : joints_position_)
	{
		leg_joint_id = 0;
		int idx = leg_id*leg.key_->getNJoints();
		for(auto &joint : *leg.data_)
		{
			blind_state_msg.joints_name()[idx+leg_joint_id] = joints_name_[joint.key_];
			blind_state_msg.joints_position()[idx+leg_joint_id] = joints_position_[joint.key_];
			blind_state_msg.joints_velocity()[idx+leg_joint_id] = joints_velocity_[joint.key_];
			blind_state_msg.joints_acceleration()[idx+leg_joint_id] = joints_acceleration_[joint.key_];
			blind_state_msg.joints_effort()[idx+leg_joint_id] = joints_effort_[joint.key_];
			blind_state_msg.joints_temperature()[idx+leg_joint_id] = joints_temperature_[joint.key_];
			leg_joint_id++;
		}

		int idx_xyz = leg_id*3;
		for(unsigned int i{0}; i<3; i++)
		{
			blind_state_msg.feet_position()[idx_xyz + i] = feet_position_[leg.key_][i];
			blind_state_msg.feet_velocity()[idx_xyz + i] = feet_velocity_[leg.key_][i];
			blind_state_msg.feet_acceleration()[idx_xyz + i] = feet_acceleration_[leg.key_][i];
		}
		
		blind_state_msg.stance_legs()[leg_id] = stance_legs_[leg.key_];
		leg_id++;
	}

	for(unsigned int i{0}; i<3; i++)
	{
		blind_state_msg.terrain_inclination()[i] = terrain_inclination_[i];
		blind_state_msg.surface_normal()[i] = surface_normal_[i];

		blind_state_msg.base_position_world()[i] = base_pose_world_.toPosition()[i];
		blind_state_msg.base_orientation_world_rpy()[i] = base_orientation_world_rpy_[i];
		blind_state_msg.base_linear_velocity_world()[i] = base_velocity_world_.getLinear()[i];
		blind_state_msg.base_angular_velocity_world()[i] = base_velocity_world_.getAngular()[i];
		blind_state_msg.base_linear_acceleration_world()[i] = base_acceleration_world_.getLinear()[i];
		blind_state_msg.base_angular_acceleration_world()[i] = base_acceleration_world_.getAngular()[i];
	}

	blind_state_msg.base_orientation_world()[0] = base_pose_world_.toQuaternion().x();
	blind_state_msg.base_orientation_world()[1] = base_pose_world_.toQuaternion().y();
	blind_state_msg.base_orientation_world()[2] = base_pose_world_.toQuaternion().z();
	blind_state_msg.base_orientation_world()[3] = base_pose_world_.toQuaternion().w();

    return blind_state_msg;
}

BlindState& BlindState::operator=(const BlindStateMsg& blind_state_msg)
{
	frame_id_ = blind_state_msg.frame_id();
	sequence_id_ = blind_state_msg.sequence_id();
	timestamp_ = blind_state_msg.timestamp();

	robot_name_ = blind_state_msg.robot_name();

	int leg_id{0};
	for(auto &leg : joints_position_)
	{
		int i = leg_id*leg.key_->getNJoints();

		for(auto &joint : *leg.data_)
		{
			joints_name_[joint.key_] = blind_state_msg.joints_name()[i];
			joints_position_[joint.key_] = blind_state_msg.joints_position()[i];
			joints_velocity_[joint.key_] = blind_state_msg.joints_velocity()[i];
			joints_acceleration_[joint.key_] = blind_state_msg.joints_acceleration()[i];
			joints_effort_[joint.key_] = blind_state_msg.joints_effort()[i];
			joints_temperature_[joint.key_] = blind_state_msg.joints_temperature()[i];
			i++;
		}

		int idx_xyz = leg_id*3;
		for(unsigned int i{0}; i<3; i++)
		{
			feet_position_[leg.key_][i] = blind_state_msg.feet_position()[idx_xyz+i];
			feet_velocity_[leg.key_][i] = blind_state_msg.feet_velocity()[idx_xyz+i];
			feet_acceleration_[leg.key_][i] = blind_state_msg.feet_acceleration()[idx_xyz+i];
		}

		stance_legs_[leg.key_] = blind_state_msg.stance_legs()[leg_id];
		leg_id++;
	}

	terrain_inclination_ = Eigen::Vector3d(blind_state_msg.terrain_inclination()[0], blind_state_msg.terrain_inclination()[1], blind_state_msg.terrain_inclination()[2]);
	surface_normal_ = Eigen::Vector3d(blind_state_msg.surface_normal()[0], blind_state_msg.surface_normal()[1], blind_state_msg.surface_normal()[2]);

	base_pose_world_.set(Eigen::Vector3d(blind_state_msg.base_position_world().data()));
	base_pose_world_.set(Eigen::Quaterniond(blind_state_msg.base_orientation_world()[3], blind_state_msg.base_orientation_world()[0], blind_state_msg.base_orientation_world()[1], blind_state_msg.base_orientation_world()[2]));
	base_orientation_world_rpy_ = Eigen::Vector3d(blind_state_msg.base_orientation_world_rpy()[0], blind_state_msg.base_orientation_world_rpy()[1], blind_state_msg.base_orientation_world_rpy()[2]);
	base_velocity_world_.setLinear(Eigen::Vector3d(blind_state_msg.base_linear_velocity_world().data()));
	base_velocity_world_.setAngular(Eigen::Vector3d(blind_state_msg.base_angular_velocity_world().data()));
	base_acceleration_world_.setLinear(Eigen::Vector3d(blind_state_msg.base_linear_acceleration_world().data()));
	base_acceleration_world_.setAngular(Eigen::Vector3d(blind_state_msg.base_angular_acceleration_world().data()));

	return *this;
}

BlindState& BlindState::operator=(const BlindState& blind_state)
{
	frame_id_ = blind_state.frame_id_;
	sequence_id_ = blind_state.sequence_id_;
	timestamp_ = blind_state.timestamp_;

	robot_name_ = blind_state.robot_name_;

	joints_name_ = blind_state.joints_name_;
	joints_position_ = blind_state.joints_position_;
	joints_velocity_ = blind_state.joints_velocity_;
	joints_acceleration_ = blind_state.joints_acceleration_;
	joints_effort_ = blind_state.joints_effort_;
	joints_temperature_ = blind_state.joints_temperature_;

	feet_position_ = blind_state.feet_position_;
	feet_velocity_ = blind_state.feet_velocity_;
	feet_acceleration_ = blind_state.feet_acceleration_;

	terrain_inclination_ = blind_state.terrain_inclination_;
	surface_normal_ = blind_state.surface_normal_;

	base_pose_world_ = blind_state.base_pose_world_;
	base_orientation_world_rpy_ = blind_state.base_orientation_world_rpy_;
	base_velocity_world_ = blind_state.base_velocity_world_;
	base_acceleration_world_ = blind_state.base_acceleration_world_;

	stance_legs_ = blind_state.stance_legs_;

	return *this;
}