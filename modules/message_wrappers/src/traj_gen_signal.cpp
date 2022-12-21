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
    : foot_pos(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , foot_pos_HF(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , foot_vel(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , foot_vel_HF(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , foot_acc(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , nom_touch_down(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , touch_down(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , normal_force_max(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , normal_force_min(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , swing_period(pRobot->makeLegDataMap<double>(0.0))
    , stance(pRobot->makeLegDataMap<bool>(false))
{}

TrajGenSignal::TrajGenSignal(const TrajGenSignal& from)
    : foot_pos(from.foot_pos)
    , foot_pos_HF(from.foot_pos_HF)
    , foot_vel(from.foot_vel)
    , foot_vel_HF(from.foot_vel_HF)
    , foot_acc(from.foot_acc)
    , nom_touch_down(from.nom_touch_down)
    , touch_down(from.touch_down)
    , normal_force_max(from.normal_force_max)
    , normal_force_min(from.normal_force_min)
    , swing_period(from.swing_period)
    , stance(from.stance)
{ }

TrajGenSignal::~TrajGenSignal()
{ }

TrajGenSignal::operator TrajGenMsg() const
{
    TrajGenMsg msg;

    int i = 0;
    for(auto &foot_pair : this->foot_pos)
	{
	 	msg.foot_pos()[i*3] = this->foot_pos[foot_pair.key_][0];
        msg.foot_pos()[i*3 + 1] = this->foot_pos[foot_pair.key_][1];
        msg.foot_pos()[i*3 + 2] = this->foot_pos[foot_pair.key_][2];

        msg.foot_pos_HF()[i*3] = this->foot_pos_HF[foot_pair.key_][0];
        msg.foot_pos_HF()[i*3 + 1] = this->foot_pos_HF[foot_pair.key_][1];
        msg.foot_pos_HF()[i*3 + 2] = this->foot_pos_HF[foot_pair.key_][2];

        msg.foot_vel()[i*3] = this->foot_vel[foot_pair.key_][0];
        msg.foot_vel()[i*3 + 1] = this->foot_vel[foot_pair.key_][1];
        msg.foot_vel()[i*3 + 2] = this->foot_vel[foot_pair.key_][2];

        msg.foot_vel_HF()[i*3] = this->foot_vel_HF[foot_pair.key_][0];
        msg.foot_vel_HF()[i*3 + 1] = this->foot_vel_HF[foot_pair.key_][1];
        msg.foot_vel_HF()[i*3 + 2] = this->foot_vel_HF[foot_pair.key_][2];

        msg.foot_acc()[i*3] = this->foot_acc[foot_pair.key_][0];
        msg.foot_acc()[i*3 + 1] = this->foot_acc[foot_pair.key_][1];
        msg.foot_acc()[i*3 + 2] = this->foot_acc[foot_pair.key_][2];

        msg.nom_touch_down()[i*3] = this->nom_touch_down[foot_pair.key_][0];
        msg.nom_touch_down()[i*3 + 1] = this->nom_touch_down[foot_pair.key_][1];
        msg.nom_touch_down()[i*3 + 2] = this->nom_touch_down[foot_pair.key_][2];

        msg.touch_down()[i*3] = this->touch_down[foot_pair.key_][0];
        msg.touch_down()[i*3 + 1] = this->touch_down[foot_pair.key_][1];
        msg.touch_down()[i*3 + 2] = this->touch_down[foot_pair.key_][2];

        msg.swing_period()[i] = this->swing_period[foot_pair.key_];

        msg.stance()[i] = this->stance[foot_pair.key_];
        i++;
    }
    
    return msg;
}

TrajGenSignal &TrajGenSignal::operator= (TrajGenMsg &msg)
{
    int i = 0;
    for(auto &foot_pair : this->foot_pos)
	{
	 	this->foot_pos[foot_pair.key_][0] = msg.foot_pos()[i*3];
        this->foot_pos[foot_pair.key_][1] = msg.foot_pos()[i*3 + 1];
        this->foot_pos[foot_pair.key_][2] = msg.foot_pos()[i*3 + 2];

        this->foot_pos_HF[foot_pair.key_][0] = msg.foot_pos_HF()[i*3];
        this->foot_pos_HF[foot_pair.key_][1] = msg.foot_pos_HF()[i*3 + 1];
        this->foot_pos_HF[foot_pair.key_][2] = msg.foot_pos_HF()[i*3 + 2];

        this->foot_vel[foot_pair.key_][0] = msg.foot_vel()[i*3];
        this->foot_vel[foot_pair.key_][1] = msg.foot_vel()[i*3 + 1];
        this->foot_vel[foot_pair.key_][2] = msg.foot_vel()[i*3 + 2];

        this->foot_vel_HF[foot_pair.key_][0] = msg.foot_vel_HF()[i*3];
        this->foot_vel_HF[foot_pair.key_][1] = msg.foot_vel_HF()[i*3 + 1];
        this->foot_vel_HF[foot_pair.key_][2] = msg.foot_vel_HF()[i*3 + 2];

        this->foot_acc[foot_pair.key_][0] = msg.foot_acc()[i*3];
        this->foot_acc[foot_pair.key_][1] = msg.foot_acc()[i*3 + 1];
        this->foot_acc[foot_pair.key_][2] = msg.foot_acc()[i*3 + 2];

        this->nom_touch_down[foot_pair.key_][0] = msg.nom_touch_down()[i*3];
        this->nom_touch_down[foot_pair.key_][1] = msg.nom_touch_down()[i*3 + 1];
        this->nom_touch_down[foot_pair.key_][2] = msg.nom_touch_down()[i*3 + 2];

        this->touch_down[foot_pair.key_][0] = msg.touch_down()[i*3];
        this->touch_down[foot_pair.key_][1] = msg.touch_down()[i*3 + 1];
        this->touch_down[foot_pair.key_][2] = msg.touch_down()[i*3 + 2];

        this->swing_period[foot_pair.key_] = msg.swing_period()[i];

        this->stance[foot_pair.key_] = msg.stance()[i];
        i++;
    }
	
    return *this;
}
#endif // TRAJ_GEN_SIGNAL_CPP
