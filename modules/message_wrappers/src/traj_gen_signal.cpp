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
    , swing_period(pRobot->makeLegDataMap<double>(0.0))
    , stance(pRobot->makeLegDataMap<bool>(false))
{}

TrajGenSignal::~TrajGenSignal()
{}

#endif // TRAJ_GEN_SIGNAL_CPP
