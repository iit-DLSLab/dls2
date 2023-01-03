/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/

#ifndef CONTROLLER_COMMAND_CPP
#define CONTROLLER_COMMAND_CPP


#include "dls2/msg_wrappers/controller_command.hpp"

using namespace dls;

ControllerCommand::ControllerCommand(const std::shared_ptr<robotlib::RobotBase>& pRobot)
    : robot_height{0.3}
    , step_frequency{0.5}
    , duty_factor{0.55}
    , step_height(pRobot->makeLegDataMap<double>(0.08))
    , base_pose_HF(Eigen::Vector3d::Zero(), Eigen::Quaterniond::Identity())
    , base_vel_HF(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero())
{}

ControllerCommand::ControllerCommand(ControllerCommand& from)
    : robot_height(from.robot_height)
    , step_frequency(from.step_frequency)
    , duty_factor(from.duty_factor)
    , step_height(from.step_height)
    , base_pose_HF(from.base_pose_HF)
    , base_vel_HF(from.base_vel_HF)
{}

ControllerCommand::~ControllerCommand()
{}

ControllerCommand::operator ControllerCommandMsg() const
{
    ControllerCommandMsg msg;
    msg.robot_height() = this->robot_height;
    msg.step_frequency() = this->step_frequency;
    msg.duty_factor() = this->duty_factor;
    int i {0};
    for(auto &leg_pair : this->step_height)
    {
        msg.step_height()[i] = this->step_height[leg_pair.key_];
    }
    for(int i=0;i<3;i++)
    {
        msg.base_pos_HF()[i] = this->base_pose_HF.toPosition()(i);
        msg.base_lin_vel_HF()[i] = this->base_vel_HF.getLinear()(i);
        msg.base_ang_vel_HF()[i] = this->base_vel_HF.getAngular()(i);
    }

    msg.base_ori_HF()[0] = this->base_pose_HF.toQuaternion().x();
	msg.base_ori_HF()[1] = this->base_pose_HF.toQuaternion().y();
	msg.base_ori_HF()[2] = this->base_pose_HF.toQuaternion().z();
	msg.base_ori_HF()[3] = this->base_pose_HF.toQuaternion().w();
    return msg;
}

ControllerCommand& ControllerCommand::operator= (const ControllerCommandMsg &msg)
{
    this->robot_height = msg.robot_height();
    this->step_frequency = msg.step_frequency();
    this->duty_factor = msg.duty_factor();
    
    int i {0};
    for(auto &leg_pair : this->step_height)
    {
        this->step_height[leg_pair.key_] = msg.step_height()[i];
    }
    for(int i=0;i<3;i++)
    {
        this->base_pose_HF.set(Eigen::Vector3d((msg.base_pos_HF().data())));
        this->base_pose_HF.set(Eigen::Quaterniond(
                                            msg.base_ori_HF()[3],
                                            msg.base_ori_HF()[0],
                                            msg.base_ori_HF()[1],
                                            msg.base_ori_HF()[2]));
	    this->base_vel_HF.setLinear(Eigen::Vector3d(msg.base_lin_vel_HF().data()));
	    this->base_vel_HF.setAngular(Eigen::Vector3d(msg.base_ang_vel_HF().data()));
    }
    return *this;
}

#endif // CONTROLLER_COMMAND_CPP