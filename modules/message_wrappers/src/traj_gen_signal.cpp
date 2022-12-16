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

#ifndef TRAJ_GEN_SIGNAL_CPP
#define TRAJ_GEN_SIGNAL_CPP


#include "dls2/msg_wrappers/traj_gen_signal.hpp"

using namespace dls;

TrajGenSignal::TrajGenSignal(const std::shared_ptr<robotlib::RobotBase>& pRobot)
    : foot(pRobot->makeLegDataMap<FootState>(FootState::Zero()))
    , nom_touch_down(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , touch_down(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , swing_period(pRobot->makeLegDataMap<double>(0.0))
    , stance(pRobot->makeLegDataMap<bool>(false))
{}

TrajGenSignal::~TrajGenSignal()
{}

TrajGenSignal::operator TrajGenMsg() const
{
    TrajGenMsg msg;

    int i = 0;
    for(auto &foot_pair : this->foot)
	{
	 	msg.foot_pos()[i*3] = this->foot[foot_pair.key_].pos[0];
        msg.foot_pos()[i*3 + 1] = this->foot[foot_pair.key_].pos[1];
        msg.foot_pos()[i*3 + 2] = this->foot[foot_pair.key_].pos[2];

        msg.stance()[i] = this->stance[foot_pair.key_];
        i++;
        std::cout << "##### Key: " << foot_pair.key_ << std::endl;
    }
    
    return msg;
}

TrajGenSignal &TrajGenSignal::operator= (TrajGenMsg &msg)
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
#endif // TRAJ_GEN_SIGNAL_CPP
