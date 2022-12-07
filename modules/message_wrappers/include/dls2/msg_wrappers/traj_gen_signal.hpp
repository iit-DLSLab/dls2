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

namespace Eigen
{
    typedef Eigen::Matrix<double,6,1> Vector6d;
}

namespace dls
{
    class  TrajGenSignal {
    public:
        TrajGenSignal(const std::shared_ptr<robotlib::RobotBase>);
        TrajGenSignal() = delete;
        ~TrajGenSignal();

        robotlib::LegDataMap<FootState> foot;
        robotlib::LegDataMap<Eigen::Vector3d> nom_touch_down;
        robotlib::LegDataMap<Eigen::Vector3d> touch_down;
        robotlib::LegDataMap<double> step_cycle_phase;
        robotlib::LegDataMap<double> swing_cycle_phase;
        robotlib::LegDataMap<double> stance_cycle_phase;
        robotlib::LegDataMap<double> swing_period;
        robotlib::LegDataMap<double> stance_period;
        robotlib::LegDataMap<bool> stance;

        robotlib::LegDataMap<Eigen::Vector3d> ffwdTorques;
        robotlib::LegDataMap<double> normal_force_max;
        robotlib::LegDataMap<double> normal_force_min;

        Eigen::Vector6d ffwdWrench = Eigen::Vector6d::Zero();
    };
}
#endif // TRAJ_GEN_SIGNAL_HPP