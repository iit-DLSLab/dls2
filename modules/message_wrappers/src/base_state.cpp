
#include "dls2/msg_wrappers/base_state.hpp"

BaseState::BaseState(const std::shared_ptr<robotlib::RobotBase> robot)
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, robot_name_(robot->getName())
	, pose_(Eigen::Vector3d::Zero(), Eigen::Quaterniond::Identity())
	, velocity_(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero())
	, acceleration_(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero())
	, stance_status_(robot->makeLegDataMap<bool>(false))
{}

BaseState::BaseState(BaseState& base_state)
    : frame_id_(base_state.frame_id_)
	, sequence_id_(base_state.sequence_id_)
	, timestamp_(base_state.timestamp_)
	, robot_name_(base_state.robot_name_)
	, pose_(base_state.pose_)
	, velocity_(base_state.velocity_)
	, acceleration_(base_state.acceleration_)
	, stance_status_(base_state.stance_status_)
{}

BaseState::~BaseState(){}

BaseState::operator BaseStateMsg() const
{
    BaseStateMsg base_state_msg;

	base_state_msg.frame_id(frame_id_);
	base_state_msg.sequence_id(sequence_id_);
	base_state_msg.timestamp(timestamp_);

	base_state_msg.robot_name(robot_name_);

	for(unsigned int i{0}; i<3; i++)
	{
		base_state_msg.position()[i] = pose_.toPosition()[i];
		base_state_msg.linear_velocity()[i] = velocity_.getLinear()[i];
		base_state_msg.angular_velocity()[i] = velocity_.getAngular()[i];
		base_state_msg.linear_acceleration()[i] = acceleration_.getLinear()[i];
		base_state_msg.angular_acceleration()[i] = acceleration_.getAngular()[i];
	}

	base_state_msg.orientation()[0] = pose_.toQuaternion().x();
	base_state_msg.orientation()[1] = pose_.toQuaternion().y();
	base_state_msg.orientation()[2] = pose_.toQuaternion().z();
	base_state_msg.orientation()[3] = pose_.toQuaternion().w();

	int i{0};
	for (auto &leg : stance_status_)
	{
		base_state_msg.stance_status()[i] = stance_status_[leg.key_];
		i++;
	}

    return base_state_msg;
}

BaseState& BaseState::operator=(const BaseStateMsg& base_state_msg)
{
	frame_id_ = base_state_msg.frame_id();
	sequence_id_ = base_state_msg.sequence_id();
	timestamp_ = base_state_msg.timestamp();

	robot_name_ = base_state_msg.robot_name();

	pose_.set(Eigen::Vector3d(base_state_msg.position().data()));
	pose_.set(Eigen::Quaterniond(base_state_msg.orientation()[3], base_state_msg.orientation()[0], base_state_msg.orientation()[1], base_state_msg.orientation()[2]));
	velocity_.setLinear(Eigen::Vector3d(base_state_msg.linear_velocity().data()));
	velocity_.setAngular(Eigen::Vector3d(base_state_msg.angular_velocity().data()));
	acceleration_.setLinear(Eigen::Vector3d(base_state_msg.linear_acceleration().data()));
	acceleration_.setAngular(Eigen::Vector3d(base_state_msg.angular_acceleration().data()));

	int i{0};
	for (auto &leg : stance_status_)
	{
		stance_status_[leg.key_] = base_state_msg.stance_status()[i];
		i++;
	}

	return *this;
}

BaseState& BaseState::operator=(const BaseState& base_state)
{
    frame_id_ = base_state.frame_id_;
	sequence_id_ = base_state.sequence_id_;
	timestamp_ = base_state.timestamp_;

	robot_name_ = base_state.robot_name_;

	pose_ = base_state.pose_;
	velocity_ = base_state.velocity_;
	acceleration_ = base_state.acceleration_;

	stance_status_ = base_state.stance_status_;

	return *this;
}

void BaseState::setDataFromWrapperBase(WrapperBase* wrapper_base)
{
    // Assuming wrapper is of type Wrapper<MsgType>
    *this = static_cast<BaseState&>(*wrapper_base);
}