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
    , des_base_pos_HF(Eigen::Matrix<double, 6, 1>::Zero())
    , des_base_vel_HF(Eigen::Matrix<double, 6, 1>::Zero())
{}

ControllerCommand::ControllerCommand(ControllerCommand& from)
    : robot_height(from.robot_height)
    , step_frequency(from.step_frequency)
    , duty_factor(from.duty_factor)
    , step_height(from.step_height)
    , des_base_pos_HF(from.des_base_pos_HF)
    , des_base_vel_HF(from.des_base_vel_HF)
{}

ControllerCommand::~ControllerCommand()
{}

ControllerCommand::operator ControllerCommandMsg() const
{
    ControllerCommandMsg msg;

    // const double *p = this->desired_com_pose_world.toQuaternion().coeffs().data();
	// std::copy(p, p + 4, msg.com_ori().begin());
    
    // for(int i = 0; i < 3; i++)
    // {
    //     msg.com_pos()[i] = this->desired_com_pose_world.toPosition()[i];
    //     msg.com_lin_vel()[i] = this->desired_com_velocity_world.getLinear()[i];
    //     msg.com_ang_vel()[i] = this->desired_com_velocity_world.getAngular()[i];
    //     msg.com_lin_acc()[i] = this->desired_com_acceleration_world.getLinear()[i];
    //     msg.com_ang_acc()[i] = this->desired_com_acceleration_world.getAngular()[i];
    // }
    
    // // msg.desired_base_pose_world(this->desired_base_pose_world);
    // // msg.desired_base_velocity_world(this->desired_base_velocity_world);
    // // msg.desired_base_acceleration_world(this->desired_base_acceleration_world);

    // int i = 0;
	// for(auto &leg_pair : this->desired_joint_position)
	// {
	// 	for(auto &joint : *leg_pair.data_)
    //     {
    //         msg.joint_pos()[i] = this->desired_joint_position[joint.key_];
    //         msg.joint_vel()[i] = this->desired_joint_velocity[joint.key_];
    //         msg.joint_acc()[i] = this->desired_joint_acceleration[joint.key_];
    //         msg.joint_eff()[i] = this->desired_joint_effort[joint.key_];
    //         i++;
    //     }
    // }

    // i = 0;
    // for(auto &leg_pair : this->stance_legs)
	// {
    // 	msg.stance_feet()[i] = *leg_pair.data_;
    //     i++;
    // }

	// // msg.desired_base_wrench(this->desired_base_wrench);

    return msg;
}

ControllerCommand& ControllerCommand::operator= (const ControllerCommandMsg &msg)
{
    // desired_com_pose_world.set(Eigen::Vector3d(msg.com_pos().data()), Eigen::Quaterniond(msg.com_ori().data())),
    // desired_com_velocity_world.setLinear(Eigen::Vector3d(msg.com_lin_vel().data()));
    // desired_com_velocity_world.setAngular(Eigen::Vector3d(msg.com_ang_vel().data()));
    // desired_com_acceleration_world.setLinear(Eigen::Vector3d(msg.com_lin_acc().data()));
    // desired_com_acceleration_world.setAngular(Eigen::Vector3d(msg.com_ang_acc().data()));

    // // desired_base_pose_world = msg.desired_base_pose_world();
    // // desired_base_velocity_world = msg.desired_base_velocity_world();
    // // desired_base_acceleration_world = msg.desired_base_acceleration_world();

    // int i = 0;
    // for(auto &leg_pair : this->desired_joint_position)
	// {
	// 	for(auto &joint : *leg_pair.data_)
    //     {
    //         this->desired_joint_position[joint.key_] = msg.joint_pos()[i];
    //         this->desired_joint_velocity[joint.key_] = msg.joint_vel()[i];
    //         this->desired_joint_acceleration[joint.key_] = msg.joint_acc()[i];
    //         this->desired_joint_effort[joint.key_] = msg.joint_eff()[i];
    //         i++;
    //     }
    // }

    // i = 0;
    // for(auto &leg_pair : this->stance_legs)
	// {
    // 	msg.stance_feet()[i] = *leg_pair.data_;
    //     i++;
    // }
	
    return *this;
}

#endif // CONTROLLER_COMMAND_CPP