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

#ifndef TRAJ_GEN_SIGNAL_HPP
#define TRAJ_GEN_SIGNAL_HPP

#include <robotlib/robot_base.hpp>
#include "dls2/msg_wrappers/foot_state.hpp"
#include "dls_messages/dds/traj_gen.h"
#include "dls2/msg_wrappers/wrapper.hpp"

namespace dls
{
    class  TrajGenSignal : public Wrapper<TrajGenMsg>
    {
    public:
        TrajGenSignal(const std::shared_ptr<robotlib::RobotBase>&);
        TrajGenSignal(const TrajGenSignal&);
        TrajGenSignal() = delete;
        ~TrajGenSignal();

        operator TrajGenMsg() const override;
		TrajGenSignal& operator= (const TrajGenMsg&) override;

    // private:
        robotlib::LegDataMap<Eigen::Vector3d> foot_pos;
        robotlib::LegDataMap<Eigen::Vector3d> foot_pos_HF;
        robotlib::LegDataMap<Eigen::Vector3d> foot_vel;
        robotlib::LegDataMap<Eigen::Vector3d> foot_vel_HF;
        robotlib::LegDataMap<Eigen::Vector3d> foot_acc;
        robotlib::LegDataMap<Eigen::Vector3d> nom_touch_down;
        robotlib::LegDataMap<Eigen::Vector3d> touch_down;
        robotlib::LegDataMap<double> normal_force_max;
        robotlib::LegDataMap<double> normal_force_min;
        Eigen::Matrix<double, 6, 1> ffwdWrench;
        robotlib::LegDataMap<double> swing_period;
        robotlib::LegDataMap<bool> stance;
    };
}
#endif // TRAJ_GEN_SIGNAL_HPP