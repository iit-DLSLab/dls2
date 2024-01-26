#include "dls2/msg_wrappers/controller_command.hpp"

ControllerCommand::ControllerCommand(const std::shared_ptr<robotlib::RobotBase> robot)
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
    , robot_height_(0.3)
    , step_frequency_(0.5)
    , duty_factor_(0.55)
    , step_height_(robot->makeLegDataMap<double>(0.08))
    , base_pose_HF_(Eigen::Vector3d::Zero(), Eigen::Quaterniond::Identity())
    , base_velocity_HF_(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero())
{
    base_pose_HF_.set(Eigen::Vector3d(0.0,0.0,robot_height_));
}

ControllerCommand::ControllerCommand(ControllerCommand& controller_command)
    : frame_id_(controller_command.frame_id_)
	, sequence_id_(controller_command.sequence_id_)
	, timestamp_(controller_command.timestamp_)
    , robot_height_(controller_command.robot_height_)
    , step_frequency_(controller_command.step_frequency_)
    , duty_factor_(controller_command.duty_factor_)
    , step_height_(controller_command.step_height_)
    , base_pose_HF_(controller_command.base_pose_HF_)
    , base_velocity_HF_(controller_command.base_velocity_HF_)
{}

ControllerCommand::~ControllerCommand(){}

ControllerCommand::operator ControllerCommandMsg() const
{
    ControllerCommandMsg controller_command_msg;

	controller_command_msg.frame_id(frame_id_);
	controller_command_msg.sequence_id(sequence_id_);
	controller_command_msg.timestamp(timestamp_);

    controller_command_msg.robot_height() = robot_height_;
    controller_command_msg.step_frequency() = step_frequency_;
    controller_command_msg.duty_factor() = duty_factor_;
    
    int i {0};
    for(auto &leg_pair : step_height_)
    {
        controller_command_msg.step_height()[i++] = step_height_[leg_pair.key_];
    }

    for(unsigned int i{0}; i<3; i++)
    {
        controller_command_msg.base_position_HF()[i] = base_pose_HF_.toPosition()(i);
        controller_command_msg.base_linear_velocity_HF()[i] = base_velocity_HF_.getLinear()(i);
        controller_command_msg.base_angular_velocity_HF()[i] = base_velocity_HF_.getAngular()(i);
    }

    controller_command_msg.base_orientation_HF()[0] = base_pose_HF_.toQuaternion().x();
	controller_command_msg.base_orientation_HF()[1] = base_pose_HF_.toQuaternion().y();
	controller_command_msg.base_orientation_HF()[2] = base_pose_HF_.toQuaternion().z();
	controller_command_msg.base_orientation_HF()[3] = base_pose_HF_.toQuaternion().w();

    return controller_command_msg;
}

ControllerCommand& ControllerCommand::operator= (const ControllerCommandMsg &controller_command_msg)
{
    frame_id_ = controller_command_msg.frame_id();
    sequence_id_ = controller_command_msg.sequence_id();
    timestamp_ = controller_command_msg.timestamp();

    robot_height_ = controller_command_msg.robot_height();
    step_frequency_ = controller_command_msg.step_frequency();
    duty_factor_ = controller_command_msg.duty_factor();
    
    int i {0};
    for(auto &leg_pair : step_height_)
    {
        step_height_[leg_pair.key_] = controller_command_msg.step_height()[i++];
    }

    for(int i{0}; i < 3; i++)
    {
        base_pose_HF_.set(Eigen::Vector3d((controller_command_msg.base_position_HF().data())));
        base_pose_HF_.set(Eigen::Quaterniond(controller_command_msg.base_orientation_HF()[3],
                                             controller_command_msg.base_orientation_HF()[0],
                                             controller_command_msg.base_orientation_HF()[1],
                                             controller_command_msg.base_orientation_HF()[2]));
	    base_velocity_HF_.setLinear(Eigen::Vector3d(controller_command_msg.base_linear_velocity_HF().data()));
	    base_velocity_HF_.setAngular(Eigen::Vector3d(controller_command_msg.base_angular_velocity_HF().data()));
    }

    return *this;
}

ControllerCommand& ControllerCommand::operator=(const ControllerCommand& controller_command)
{
    frame_id_ = controller_command.frame_id_;
	sequence_id_ = controller_command.sequence_id_;
	timestamp_ = controller_command.timestamp_;

    robot_height_ = controller_command.robot_height_;
    step_frequency_ = controller_command.step_frequency_;
    duty_factor_ = controller_command.duty_factor_;
    step_height_ = controller_command.step_height_;

    base_pose_HF_ = controller_command.base_pose_HF_;
    base_velocity_HF_ = controller_command.base_velocity_HF_;

	return *this;
}

void ControllerCommand::setDataFromWrapperBase(WrapperBase* wrapper_base)
{
    // Assuming wrapper is of type Wrapper<MsgType>
    *this = static_cast<ControllerCommand&>(*wrapper_base);
}