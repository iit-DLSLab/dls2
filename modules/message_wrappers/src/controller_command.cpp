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

ControllerCommand::ControllerCommand(const std::shared_ptr<robotlib::RobotBase> pRobot)
    : robot_height{0.3}
    , step_frequency{0.5}
    , duty_factor{0.55}
    , step_height(pRobot->makeLegDataMap<double>(0.08))
    , des_base_pos_HF(Eigen::Matrix<double, 6, 1>::Zero())
    , des_base_vel_HF(Eigen::Matrix<double, 6, 1>::Zero())
{}

ControllerCommand::~ControllerCommand()
{}

#endif // CONTROLLER_COMMAND_CPP
