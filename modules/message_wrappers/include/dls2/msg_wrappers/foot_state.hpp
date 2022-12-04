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
#ifndef FOOT_STATE_HPP
#define FOOT_STATE_HPP

#include <robotlib/robot_base.hpp>

namespace dls
{
    class FootState {
    public:
        FootState(const std::shared_ptr<robotlib::RobotBase> robot);
        FootState() = delete;
        ~FootState();

        double proprio_height;
        robotlib::LegDataMap<Eigen::Vector3d> pos;
        robotlib::LegDataMap<Eigen::Vector3d> pos_HF;
        robotlib::LegDataMap<Eigen::Vector3d> vel;
        robotlib::LegDataMap<Eigen::Vector3d> acc;
    };
}
#endif // FOOT_STATE_HPP