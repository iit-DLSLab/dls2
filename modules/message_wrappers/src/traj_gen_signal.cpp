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

TrajGenSignal::TrajGenSignal(const std::shared_ptr<robotlib::RobotBase> pRobot)
    : foot(pRobot->makeLegDataMap<FootState>(FootState::Zero()))
    , nom_touch_down(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , touch_down(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , step_cycle_phase(pRobot->makeLegDataMap<double>(0.0))
    , swing_cycle_phase(pRobot->makeLegDataMap<double>(0.0))
    , stance_cycle_phase(pRobot->makeLegDataMap<double>(0.0))
    , swing_period(pRobot->makeLegDataMap<double>(0.0))
    , stance_period(pRobot->makeLegDataMap<double>(0.0))
    , stance(pRobot->makeLegDataMap<bool>(false))
    , ffwdTorques(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , normal_force_max(pRobot->makeLegDataMap<double>(800.0))
    , normal_force_min(pRobot->makeLegDataMap<double>(5.0))
{}

TrajGenSignal::~TrajGenSignal()
{}

#endif // TRAJ_GEN_SIGNAL_CPP
